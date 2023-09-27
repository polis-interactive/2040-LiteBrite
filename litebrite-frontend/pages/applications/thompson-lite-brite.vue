<script setup lang="ts">
import { AppPage } from '~/lib/domain/app';
import { useAppStore } from "~/stores/app"

const appPages: Array<AppPage> = [
  {
    display: 'Display',
    slug: 'display'
  }
]

const appStore = useAppStore();
appStore.setAppPages(appPages);
</script>


<template>
  <NuxtPage />
</template>