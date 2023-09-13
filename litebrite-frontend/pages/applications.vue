<script setup lang="ts">
import { AsyncTimeout } from '~/lib/utils'

const loading = ref(false);

definePageMeta({
  layout: 'centered'
})
</script>

<template>
  <div class="login__wrapper">
    <v-card
      class="login__body"
      min-width="400px"
      variant="outlined"
      :loading="loading"
    >
      <v-card-title 
        v-text="'Light Controller'"
        class="login__title"
      />
      <v-card-subtitle
        v-text="'Select Application'"
        class="login__title mb-2"
      />
      <v-card-text class="login__body">
        Hello world!
        <!--
        <v-list 
            color="secondary"
            nav
            :disabled="loading"
            @click:select="({ id }) => { handleSiteSelection(id as Site) }"
          >
            <template
              v-for="(site, idx) in AvaliableSites"
              :key="site.id"
            >
              <v-list-item
                :value="site"
              >
                {{ site.name }}
              </v-list-item>
            </template>
          </v-list>
          -->
      </v-card-text>
    </v-card>
  </div>

</template>

<style scoped>

</style>
