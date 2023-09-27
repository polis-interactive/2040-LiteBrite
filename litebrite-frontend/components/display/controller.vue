<script setup lang="ts">
import { storeToRefs } from 'pinia';
import _debounce from 'lodash.debounce'

import { CRGB, CRGBW } from '~/lib/domain/color';

import {DisplayValue, GetDisplayValue } from '~/lib/domain/display';

import { useDisplayStore } from '~/stores/display';

const displayStore = useDisplayStore();

const { localDisplay, displayIsLoading, localDisplayValueType, resetHack } = toRefs(displayStore);

const localRgb: Ref<CRGB> = ref({ r: 0, g: 0, b: 0 });
const localRgbw: Ref<CRGBW> = ref({ r: 0, g: 0, b: 0, w: 0 });

const localRgba: ComputedRef<CRGB & { a: number}> = computed(() => {
    const localUnref = localRgbw.value
    return { 
        r: localRgbw.value.r, 
        g: localRgbw.value.g, 
        b: localRgbw.value.b, 
        a: localRgbw.value.w / 255,
    };
})

const resetLocalColor = () => {
    switch (localDisplayValueType.value) {
        case 'RGB':
            localRgb.value = { ...localDisplay.value?.rgb_color as CRGB };
            break;
        case 'RGBW':
            localRgbw.value = { ...localDisplay.value?.rgbw_color as CRGBW };
            break;        
    }
}

watch([localDisplayValueType, resetHack], resetLocalColor);

const updateStore = _debounce(() => {
    switch (localDisplayValueType.value) {
        case 'RGB':
            displayStore.changeRgbColor({ ...localRgb.value });
            break;
        case 'RGBW':
            displayStore.changeRgbwColor({ ...localRgbw.value });
            break;        
    }
}, 300)

const handleColorUpdate = (updateType: DisplayValue, event: any) => {
    switch (updateType) {
        case 'RGB':
            localRgb.value = { ...localRgb.value, ...event}
            break;
        case 'RGBW':
            localRgbw.value = {
                r: event.r,
                g: event.g,
                b: event.b,
                w: Math.floor(event.a * 255)
            }
            break;
        case 'COLOR_TEMPERATURE':
            break;
    }
    updateStore();
}


type UpdateColorTypeValue = 'R' | 'G' | 'B' | 'RW' | 'GW' | 'BW' | 'W' | 'C' | 'CB';

const handleValueUpdate = (updateType: UpdateColorTypeValue, stringValue: string) => {
    let value = Number(stringValue)
    if (updateType != 'C') {
        value = Math.max(Math.min(value, 255), 0);
    }
    switch (updateType) {
        case 'R':
            localRgb.value.r = value;
            break;
        case 'G':
            localRgb.value.g = value;
            break;
        case 'B':
            localRgb.value.b = value;
            break;
        case 'RW':
            localRgbw.value.r = value;
            break;
        case 'GW':
            localRgbw.value.g = value;
            break;
        case 'BW':
            localRgbw.value.b = value;
            break;
        case 'W':
            localRgbw.value.w = value;
            break;
        case 'C':
        case 'CB':
    }
    updateStore();
}

onMounted(resetLocalColor)
</script>

<template>
    <div class="controller__wrapper">
        <div 
            v-if="localDisplayValueType === 'RGB'"
            class="controller__rgb__wrapper"
        >
            <v-color-picker 
                :model-value="localRgb"
                mode="rgb"
                hide-inputs
                :disabled="displayIsLoading"
                @update:model-value="handleColorUpdate('RGB', $event)"
            />
            <div class="controller__input__group">
                <v-text-field
                    :model-value="localRgb.r"
                    hide-details
                    :disabled="displayIsLoading"
                    type="number"
                    density="compact"
                    variant="solo-filled"
                    label="R"
                    @update:model-value="handleValueUpdate('R', $event)"
                />
                <v-text-field
                    v-model="localRgb.g"
                    hide-details
                    :disabled="displayIsLoading"
                    label="G"
                    type="number"
                    density="compact"
                    class="mt-2"
                    variant="solo-filled"
                />
                <v-text-field
                    v-model="localRgb.b"
                    hide-details
                    :disabled="displayIsLoading"
                    label="B"
                    type="number"
                    density="compact"
                    class="mt-2"
                    variant="solo-filled"
                />
            </div>
        </div>
        <div 
            v-else-if="localDisplayValueType === 'RGBW'"
            class="controller__rgbw__wrapper"
        >
            <v-color-picker 
                :model-value="localRgba"
                :disabled="displayIsLoading"
                mode="rgba"
                hide-inputs
                @update:model-value="handleColorUpdate('RGBW', $event)"
            />
            <div class="controller__input__group">
                <v-text-field
                    :model-value="localRgbw.r"
                    hide-details
                    :disabled="displayIsLoading"
                    type="number"
                    density="compact"
                    variant="solo-filled"
                    label="R"
                    @update:model-value="handleValueUpdate('RW', $event)"
                />
                <v-text-field
                    v-model="localRgbw.g"
                    hide-details
                    :disabled="displayIsLoading"
                    label="G"
                    type="number"
                    density="compact"
                    class="mt-2"
                    variant="solo-filled"
                    @update:model-value="handleValueUpdate('GW', $event)"
                />
                <v-text-field
                    v-model="localRgbw.b"
                    hide-details
                    :disabled="displayIsLoading"
                    label="B"
                    type="number"
                    density="compact"
                    class="mt-2"
                    variant="solo-filled"
                    @update:model-value="handleValueUpdate('BW', $event)"
                />
                <v-text-field
                    v-model="localRgbw.w"
                    hide-details
                    :disabled="displayIsLoading"
                    label="W"
                    type="number"
                    density="compact"
                    class="mt-2"
                    variant="solo-filled"
                    @update:model-value="handleValueUpdate('W', $event)"
                />
            </div>
        </div>
        <template v-else-if="localDisplayValueType === 'COLOR_TEMPERATURE'">
            CT Controller
        </template>
        <template v-else>
            Make a display type selection to continue
        </template>
    </div>
</template>

<style scoped>
.controller__rgb__wrapper, .controller__rgbw__wrapper {
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: center;
}
.controller__input__group {
    display: flex;
    flex-direction: column;
    margin-left: 1rem;
}
</style>
