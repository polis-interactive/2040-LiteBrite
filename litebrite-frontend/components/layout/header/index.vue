<script setup lang="ts">
import { storeToRefs } from 'pinia'

import ContextMenu from './context-menu.vue'

import { useSiteStore } from '~/stores/site';

const siteStore = useSiteStore();
const { currentSite } = storeToRefs(siteStore)
</script>

<template>
    <v-app-bar :elevation="2" class="header__wrapper">
        <v-container>
            <v-row no-gutters>
                <v-col>
                    <h1 class="text-h5 font-weight-medium">
                        {{  currentSite?.name }}
                    </h1>
                </v-col>
                <v-col cols="1">
                    <context-menu />
                </v-col>
            </v-row>
            <v-row no-gutters>
                Hello
            </v-row>
        </v-container>
    </v-app-bar>
</template>

<style scoped>
.header__wrapper > :deep(.v-toolbar__content) {
    height: unset !important;
    min-height: 64px;
}

</style>