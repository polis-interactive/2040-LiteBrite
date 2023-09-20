<script setup lang="ts">
import InitializerGuard from '~/components/layout/initializer-guard.vue'
import AppHeader from '~/components/layout/header/index.vue'
</script>

<template>
    <initializer-guard>
        <v-layout class="default__wrapper">
            <app-header />
            <v-main>
                <slot />
            </v-main>
        </v-layout>
    </initializer-guard>
</template>

<style scoped>
.default__wrapper {
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
}
</style>
