<script setup lang="ts">
import { useSiteStore } from '~/stores/site';
import { Site, AvaliableHosts } from '~/lib/domain/sites'

const store = useSiteStore();

const loading = ref(false);

const subtitle = computed(() => {
  return store.hasNoSite ? 'Select Domain' : 'Login'
})

function handleSiteSelection(site: Site) {
  loading.value = true;
  const currentLocation = window.location;
  const newUrl = `${currentLocation.protocol}//${site.subdomain}.${currentLocation.host}/login`;
  window.location.href = newUrl;
}

definePageMeta({
  layout: 'centered'
})
</script>

<template>
  <div class="login__wrapper">
    <v-card
      class="login__body"
      variant="outlined"
      :loading="loading"
    >
      <v-card-title 
        v-text="'Light Controller'"
        class="login__title"
      />
      <v-card-subtitle
        v-text="subtitle"
        class="login__title"
      />
      <v-card-text class="login__body">
        <template v-if="store.hasNoSite">
          <v-list 
            color="secondary"
            nav
            :disabled="loading"
            @click:select="({ id }) => { handleSiteSelection(id as Site) }"
          >
            <template
              v-for="(site, idx) in AvaliableHosts"
              :key="site.id"
            >
              <v-list-item
                :value="site"
              >
                {{ site.name }}
              </v-list-item>
            </template>
          </v-list>
        </template>
        <template v-else>
          Hello world!
        </template>
      </v-card-text>
    </v-card>
  </div>

</template>

<style scoped>
.login__wrapper {

}
</style>
