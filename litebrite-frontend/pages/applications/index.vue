<script setup lang="ts">
import { storeToRefs } from 'pinia'

import { AsyncTimeout } from '~/lib/utils'
import { Site } from '~/lib/domain/sites'

import { useSiteStore } from '~/stores/site';

const siteStore = useSiteStore();
const { availableSites } = storeToRefs(siteStore)

const loading = ref(false);

const handleSiteSelection = async (site: Site) => {
  loading.value = true;
  await AsyncTimeout(500);
  await siteStore.setCurrentSiteId(site.id);
  loading.value = false;
}

definePageMeta({
  layout: 'centered'
})
</script>

<template>
    <v-card
      min-width="400px"
      variant="outlined"
      :loading="loading"
    >
      <v-card-title 
        v-text="'Light Controller'"
      />
      <v-card-subtitle
        v-text="'Select Application'"
        class="mb-2"
      />
      <v-card-text>
        <v-list 
            color="secondary"
            nav
            :disabled="loading"
          >
            <template
              v-for="(site, idx) in availableSites"
              :key="site.id"
            >
              <v-list-item
                :value="site"
                @click="handleSiteSelection(site)"
              >
                {{ site.name }}
              </v-list-item>
            </template>
          </v-list>
      </v-card-text>
    </v-card>
</template>

