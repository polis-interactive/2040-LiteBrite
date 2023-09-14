<script setup lang="ts">
import { storeToRefs } from 'pinia'

import { useAppStore } from '~/stores/app';

const appStore = useAppStore();
const { hasInitialized } = storeToRefs(appStore)
</script>

<template>
  <v-layout class="centered__layout">
    <v-main class="centered__main">
      <div
        v-if="!hasInitialized"
        class="centered__initializer"
      >
        <v-card
          class="centered__initializer__card"
          min-width="400px"
          variant="outlined"
        >
          <v-card-text class="centered__initializer__body">
            <v-progress-circular
              :size="60"
              :width="10"
              indeterminate
              color="teal"
            />
            <div class="centered__initializer__text">
              Initializing session
            </div>
          </v-card-text>
        </v-card>
      </div>
      <slot v-else />
    </v-main>
  </v-layout>
</template>

<style scoped>
.centered__layout {
    height: 100vh;
    width: 100vw;
}
.centered__main {
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
}
.centered__initializer__body {
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