<script setup lang="ts">
import { storeToRefs } from 'pinia'

import ContextMenu from './context-menu.vue'
import AppNavigation from './app-navigation.vue'

import { useSiteStore } from '~/stores/site';

const siteStore = useSiteStore();
const { currentSite } = storeToRefs(siteStore)
</script>

<template>
    <v-app-bar :elevation="2" class="header__wrapper">
        <v-app-bar-title class="text-h5 font-weight-medium">
            {{ currentSite?.name }}
        </v-app-bar-title>
        <v-spacer />
        <context-menu />        
        <template v-slot:extension>
            <app-navigation />
        </template>
    </v-app-bar>
</template>

<style scoped>
.header__wrapper > :deep(.v-toolbar__content) {
    height: unset !important;
    min-height: 64px;
}

.header__wrapper > :deep(.v-toolbar__content),
.header__wrapper > :deep(.v-toolbar__extension) {
    margin: auto;
    max-width: 1280px;
}

</style>