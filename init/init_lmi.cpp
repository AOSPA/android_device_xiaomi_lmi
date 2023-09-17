/*
 * Copyright (C) 2022 Paranoid Android
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <vector>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

using android::base::GetProperty;

void property_override(char const prop[], char const value[], bool add = true) {
    prop_info* pi;

    pi = (prop_info*)__system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void load_dalvikvm_properties() {
    struct sysinfo sys;
    sysinfo(&sys);
    if(sys.totalram > 6144ull * 1024 * 1024) {
    // from - phone-xhdpi-8192-dalvik-heap.mk
        property_override("dalvik.vm.heapstartsize", "24m");
        property_override("dalvik.vm.heaptargetutilization", "0.46");
        property_override("dalvik.vm.heapmaxfree", "48m");
    }
}

void load_redmi_k30_pro_zoom_edition() {
    property_override("ro.boot.hardware.sku", "pro");

    property_override("ro.product.brand", "Redmi");
    property_override("ro.product.device", "lmipro");
    property_override("ro.product.manufacturer", "Xiaomi");
    property_override("ro.product.model", "Redmi K30 Pro Zoom Edition");
    property_override("ro.product.name", "lmipro");
}

void load_redmi_k30_pro() {
    property_override("ro.boot.hardware.sku", "std");

    property_override("ro.product.brand", "Redmi");
    property_override("ro.product.device", "lmi");
    property_override("ro.product.manufacturer", "Xiaomi");
    property_override("ro.product.model", "Redmi K30 Pro");
    property_override("ro.product.name", "lmi");
}

void vendor_load_properties() {
    std::string variant = GetProperty("ro.boot.product.hardware.sku", "");
    if (variant.find("pro") != std::string::npos) {
        load_redmi_k30_pro_zoom_edition();
    } else {
        load_redmi_k30_pro();
    }

    load_dalvikvm_properties();
}
