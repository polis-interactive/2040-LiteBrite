<script setup lang="ts">
import InitializerGuard from '~/components/layout/initializer-guard.vue'
import AppHeader from '~/components/layout/header/index.vue'
</script>

<template>
    <initializer-guard>
        <v-layout>
            <app-header />
            <v-main>
                <slot />
            </v-main>
        </v-layout>
    </initializer-guard>
</template>

<style scoped>

</style>
