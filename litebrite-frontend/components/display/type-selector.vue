<script setup lang="ts">
import { storeToRefs } from 'pinia';

import { AllDisplayStyles, DisplayType, DisplayStyle } from '~/lib/domain/display';

const shownDisplayStyles = AllDisplayStyles.filter(({ type }: DisplayStyle) => {
    return type === 'RGB' || type === 'RGBW';
})

import { useDisplayStore } from '~/stores/display';

const displayStore = useDisplayStore();

const { localDisplay, displayIsLoading } = toRefs(displayStore);
const menuIsOpen = ref(false);

const changeLoaclDisplayType = (newDisplayType: DisplayType) => {
    displayStore.changeDisplayType(newDisplayType);
}

</script>

<template>
    <div class="type-selector__wrapper">
        <v-select
            label="Display Type"
            :model-value="localDisplay?.style.type"
            :items="shownDisplayStyles"
            :disabled="displayIsLoading"
            item-title="name"
            item-value="type"
            variant="outlined"
            :hint="localDisplay?.style.description"
            persistent-hint
            @update:model-value="changeLoaclDisplayType"
            @update:menu="(isOpen) => menuIsOpen = isOpen"
        >
            <template v-slot:append-inner>
                <fa-icon
                    :icon="['fad', 'caret-down']"
                    :class="['mr-2', menuIsOpen ? 'fa-rotate-180' : '' ]"
                />
            </template>
            <template v-slot:item="{ props, item }">
                <v-list-item v-bind="props" :subtitle="item.raw.description">
                </v-list-item>
            </template>
        </v-select>
    </div>
</template>

<style scoped>
.type-selector__wrapper {
    display: flex;
    flex-direction: row;
}
</style>
