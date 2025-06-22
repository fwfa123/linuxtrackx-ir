#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

// Simple test to verify TrackIR detection using our built library
int main() {
    printf("🔍 Testing TrackIR Detection with Modernized Libraries\n");
    printf("=======================================================\n");
    
    // Try to load the TrackIR library
    void *tir_lib = dlopen("/usr/local/lib/linuxtrack/libtir.so.0", RTLD_LAZY);
    if (!tir_lib) {
        printf("❌ Failed to load libtir.so.0: %s\n", dlerror());
        return 1;
    }
    printf("✅ Successfully loaded libtir.so.0\n");
    
    // Look for the probe function
    typedef int (*probe_func_t)(void);
    probe_func_t probe_tir = (probe_func_t) dlsym(tir_lib, "ltr_int_tir_found");
    
    if (!probe_tir) {
        printf("⚠️  Function ltr_int_tir_found not found, trying alternative...\n");
        probe_tir = (probe_func_t) dlsym(tir_lib, "probeTir");
    }
    
    if (!probe_tir) {
        printf("❌ TrackIR probe function not found: %s\n", dlerror());
        dlclose(tir_lib);
        return 1;
    }
    printf("✅ Found TrackIR probe function\n");
    
    // Test TrackIR detection
    printf("🔍 Probing for TrackIR hardware...\n");
    int result = probe_tir();
    
    if (result > 0) {
        printf("🎉 SUCCESS: TrackIR detected! (Result: %d)\n", result);
        printf("📱 Your TrackIR 5 hardware is working with the modernized libraries!\n");
    } else {
        printf("❌ TrackIR not detected (Result: %d)\n", result);
        printf("💡 This might be a permissions issue or the device needs initialization\n");
    }
    
    dlclose(tir_lib);
    printf("\n🏁 Test completed.\n");
    return 0;
} 