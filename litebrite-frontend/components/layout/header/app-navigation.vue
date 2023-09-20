<script setup lang="ts">
import { storeToRefs } from "pinia";
import { AppPage } from "~/lib/domain/app";
import { useAppStore } from "~/stores/app"

const appStore = useAppStore();
const { appPages } = storeToRefs(appStore);

const selectedPage: Ref<null | AppPage> = ref(null);

const doPageNavigation = async (newAppPage: AppPage) => {
    await appStore.setAppPage(newAppPage.slug);
}
</script>

<template>
    <v-tabs
        v-model="selectedPage"
        align-tabs="start"
        @update:model-value="doPageNavigation"
    >
        <v-tab
            v-for="page in appPages"
            :key="page.slug"
            :value="page"
        >
            {{  page.display }}
        </v-tab>
    </v-tabs>
</template>