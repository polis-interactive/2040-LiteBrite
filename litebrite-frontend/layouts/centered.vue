<script setup lang="ts">
import InitializerGuard from '~/components/layout/initializer-guard.vue'
</script>

<template>
    <initializer-guard>
        <v-layout class="centered__layout">
            <v-main class="centered__main">
                <slot />
            </v-main>
        </v-layout>
    </initializer-guard>
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