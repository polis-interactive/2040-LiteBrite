<script setup lang="ts">
import { storeToRefs } from 'pinia';

import { useDisplay } from 'vuetify'

import { AsyncTimeout } from '~/lib/utils'

import { useDisplayStore } from '~/stores/display';

import PageLoad from '~/components/layout/page-load.vue';
import DisplayTypeSelector from '~/components/display/type-selector.vue';
import DisplayController from '~/components/display/controller.vue';
import DisplayActions from '~/components/display/actions.vue';
import Toastbar from '~/components/layout/toastbar.vue'

const { xs, sm } = useDisplay();


const displayStore = useDisplayStore();

const { 
  hasLoaded, displayIsLoading
} = storeToRefs(displayStore)

onMounted(async () => {
  if (!hasLoaded.value) {
    await AsyncTimeout(500);
    await displayStore.featchRemoteDisplay();
  }
});
</script>

<template>
  <div class="display__wrapper py-10">
    <page-load v-if="!hasLoaded">
      <span>Loading Remote Display</span>
    </page-load>
    <v-card
      v-else
      class="mx-auto my-0 pa-4"
      variant="elevated"
      min-width="550"
      max-width="1280"
      :width=" xs ? '100%' : (sm ? '90%' : '80%')"
      :loading="displayIsLoading"
    >
      <v-card-text class="display__body">
        <display-type-selector class="display__body__type"/>
        <display-controller class="display__body__controller" />
      </v-card-text>
      <display-actions />
    </v-card>

    <toastbar />
  </div>
</template>

<style scoped>
.display__wrapper {
  height: 100%;
  overflow-y:auto;
}
.display__body {
  display: flex;
  flex-direction: row;
  align-items: flex-start;
}

.display__body__type {
  flex: 1 1 0;
  margin-right: 2rem;
}
.display__body__controller {
  flex: 2 1 0;
}
.display__action__icon {
  margin-right: 0.5rem;
}

@media (max-width: 960px) {
  .display__body {
    flex-direction: column;
    align-items: stretch;
  }

.display__body__type {
  flex: 1 1 0;
  margin-right: 0;
  margin-bottom: 2rem;
}
}
</style>
