#include <sys/types.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <android/log.h>
#include "zygisk.hpp"

#define LOG_TAG "SpooferPerApp"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

static jstring (*orig_native_get)(JNIEnv *, jclass, jstring) = nullptr;
static jstring (*orig_native_get_default)(JNIEnv *, jclass, jstring, jstring) = nullptr;

static const char *spoof_value(const char *key) {
    if (!key) return nullptr;
    struct Pair { const char *key; const char *value; };
    static const Pair values[] = {
        {"ro.product.manufacturer", "Apple"},
        {"ro.product.brand", "Apple"},
        {"ro.product.model", "iPhone 17 Pro Max"},
        {"ro.product.name", "iPhone18,2"},
        {"ro.product.device", "iPhone18,2"},
        {"ro.build.product", "iPhone18,2"},
        {"ro.product.product.manufacturer", "Apple"},
        {"ro.product.product.brand", "Apple"},
        {"ro.product.product.model", "iPhone 17 Pro Max"},
        {"ro.product.product.name", "iPhone18,2"},
        {"ro.product.product.device", "iPhone18,2"},
        {"ro.product.system.manufacturer", "Apple"},
        {"ro.product.system.brand", "Apple"},
        {"ro.product.system.model", "iPhone 17 Pro Max"},
        {"ro.product.system.name", "iPhone18,2"},
        {"ro.product.system.device", "iPhone18,2"},
        {"ro.product.system_ext.manufacturer", "Apple"},
        {"ro.product.system_ext.brand", "Apple"},
        {"ro.product.system_ext.model", "iPhone 17 Pro Max"},
        {"ro.product.system_ext.name", "iPhone18,2"},
        {"ro.product.system_ext.device", "iPhone18,2"},
        {"ro.product.vendor.manufacturer", "Apple"},
        {"ro.product.vendor.brand", "Apple"},
        {"ro.product.vendor.model", "iPhone 17 Pro Max"},
        {"ro.product.vendor.name", "iPhone18,2"},
        {"ro.product.vendor.device", "iPhone18,2"},
        {"ro.product.odm.manufacturer", "Apple"},
        {"ro.product.odm.brand", "Apple"},
        {"ro.product.odm.model", "iPhone 17 Pro Max"},
        {"ro.product.odm.name", "iPhone18,2"},
        {"ro.product.odm.device", "iPhone18,2"},
        {"ro.soc.manufacturer", "Apple"},
        {"ro.soc.model", "A19 Pro"},
        {"ro.vendor.soc.manufacturer", "Apple"},
        {"ro.vendor.soc.model", "A19 Pro"}
    };
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        if (strcmp(key, values[i].key) == 0) return values[i].value;
    }
    return nullptr;
}

static const char *key_utf(JNIEnv *env, jstring key, char *buffer, size_t capacity) {
    if (!key || !buffer || capacity == 0) return nullptr;
    const char *chars = env->GetStringUTFChars(key, nullptr);
    if (!chars) return nullptr;
    size_t n = strlen(chars);
    if (n >= capacity) n = capacity - 1;
    memcpy(buffer, chars, n);
    buffer[n] = '\0';
    env->ReleaseStringUTFChars(key, chars);
    return buffer;
}

static jstring hook_native_get(JNIEnv *env, jclass clazz, jstring key) {
    char name[160];
    const char *k = key_utf(env, key, name, sizeof(name));
    const char *value = spoof_value(k);
    if (value) return env->NewStringUTF(value);
    return orig_native_get ? orig_native_get(env, clazz, key) : nullptr;
}

static jstring hook_native_get_default(JNIEnv *env, jclass clazz, jstring key, jstring def) {
    char name[160];
    const char *k = key_utf(env, key, name, sizeof(name));
    const char *value = spoof_value(k);
    if (value) return env->NewStringUTF(value);
    return orig_native_get_default ? orig_native_get_default(env, clazz, key, def) : def;
}

class SpooferModule : public zygisk::ModuleBase {
public:
    void onLoad(zygisk::Api *api, JNIEnv *env) override {
        api_ = api;
        env_ = env;
    }

    void preAppSpecialize(zygisk::AppSpecializeArgs *args) override {
        if (!args || !args->nice_name) return;
        const char *process = env_->GetStringUTFChars(args->nice_name, nullptr);
        if (!process) return;
        char package[256];
        size_t n = 0;
        while (process[n] && process[n] != ':' && n < sizeof(package) - 1) {
            package[n] = process[n];
            ++n;
        }
        package[n] = '\0';
        target_ = is_target_package(package);
        env_->ReleaseStringUTFChars(args->nice_name, process);
        if (!target_) return;

        JNINativeMethod methods[] = {
            {const_cast<char *>("native_get"),
             const_cast<char *>("(Ljava/lang/String;)Ljava/lang/String;"),
             reinterpret_cast<void *>(hook_native_get)},
            {const_cast<char *>("native_get"),
             const_cast<char *>("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"),
             reinterpret_cast<void *>(hook_native_get_default)}
        };
        api_->hookJniNativeMethods(env_, "android/os/SystemProperties", methods, 2);
        orig_native_get = reinterpret_cast<decltype(orig_native_get)>(methods[0].fnPtr);
        orig_native_get_default = reinterpret_cast<decltype(orig_native_get_default)>(methods[1].fnPtr);
        LOGD("Enabled per-app identity for %s (native property hooks: %s/%s)",
             package, orig_native_get ? "ready" : "unavailable",
             orig_native_get_default ? "ready" : "unavailable");
    }

    void postAppSpecialize(const zygisk::AppSpecializeArgs *args) override {
        if (!target_) return;
        jclass build = env_->FindClass("android/os/Build");
        if (!build) {
            env_->ExceptionClear();
            LOGW("Could not find android.os.Build");
            return;
        }
        set_build_field(build, "MANUFACTURER", "Apple");
        set_build_field(build, "BRAND", "Apple");
        set_build_field(build, "MODEL", "iPhone 17 Pro Max");
        set_build_field(build, "DEVICE", "iPhone18,2");
        set_build_field(build, "PRODUCT", "iPhone18,2");
        // These are app-visible identity strings only; BOARD, HARDWARE, Android
        // release/SDK, and the real ro.board.platform are deliberately untouched.
        set_build_field(build, "SOC_MANUFACTURER", "Apple");
        set_build_field(build, "SOC_MODEL", "A19 Pro");
        env_->DeleteLocalRef(build);
        LOGD("Applied app-visible Build identity");
    }

private:
    zygisk::Api *api_ = nullptr;
    JNIEnv *env_ = nullptr;
    bool target_ = false;

    bool is_target_package(const char *package) {
        if (!api_ || !package || !package[0]) return false;
        int dirfd = api_->getModuleDir();
        if (dirfd < 0) {
            LOGW("Cannot access module directory; no app identity changes applied");
            return false;
        }
        int fd = openat(dirfd, "target_apps.txt", O_RDONLY | O_CLOEXEC);
        close(dirfd);
        if (fd < 0) {
            LOGW("target_apps.txt missing/unreadable; no app identity changes applied");
            return false;
        }
        FILE *file = fdopen(fd, "r");
        if (!file) {
            close(fd);
            return false;
        }
        char line[256];
        bool matched = false;
        while (fgets(line, sizeof(line), file)) {
            char *p = line;
            while (*p && isspace(static_cast<unsigned char>(*p))) ++p;
            if (!*p || *p == '#') continue;
            size_t len = strlen(p);
            while (len && isspace(static_cast<unsigned char>(p[len - 1]))) p[--len] = '\0';
            if (strcmp(p, package) == 0) {
                matched = true;
                break;
            }
        }
        fclose(file);
        return matched;
    }

    void set_build_field(jclass build, const char *field_name, const char *value) {
        jfieldID field = env_->GetStaticFieldID(build, field_name, "Ljava/lang/String;");
        if (!field) {
            env_->ExceptionClear();
            return; // Optional fields vary by Android release.
        }
        jstring text = env_->NewStringUTF(value);
        if (!text) {
            env_->ExceptionClear();
            return;
        }
        env_->SetStaticObjectField(build, field, text);
        env_->DeleteLocalRef(text);
        if (env_->ExceptionCheck()) env_->ExceptionClear();
    }
};

REGISTER_ZYGISK_MODULE(SpooferModule)
