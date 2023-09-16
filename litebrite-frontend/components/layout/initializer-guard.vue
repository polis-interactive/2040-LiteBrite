<script setup lang="ts">
import { storeToRefs } from 'pinia'

import { useAppStore } from '~/stores/app';

const appStore = useAppStore();
const { hasInitialized } = storeToRefs(appStore)
</script>

<template>
    <div
        v-if="!hasInitialized"
        class="initializer__wrapper"
    >
        <v-card
            class="initializer__card"
            min-width="400px"
            variant="outlined"
        >
            <v-card-text class="initializer__body">
                <v-progress-circular
                    :size="60"
                    :width="10"
                    indeterminate
                    color="teal"
                />
                <div class="initializer__text">
                    Initializing session
                </div>
            </v-card-text>
        </v-card>
    </div>
    <slot v-else />
  </template>

<style scoped>
.initializer__wrapper {
    position: absolute;
    left: 0;
    right: 0;
    top: 0;
    bottom: 0;
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
}
.initializer__body {
  display: flex;
  flex-direction: column;
  align-items: center;
  text-align: center;
  margin: 1.25rem 0;
}
.centered__initializer__text {
  margin-top: 1.25rem;
  font-size: 1.125rem;
}
</style>