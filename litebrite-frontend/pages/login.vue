<script setup lang="ts">
import { useSiteStore } from '~/stores/site';
import { Site, AvaliableHosts } from '~/lib/domain/sites'
import type { VForm } from 'vuetify/components/index'

const store = useSiteStore();

const loading = ref(false);

const email = ref("");

const emailRegex = /^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$/

const emailRules = [
  (value: string) => {
    if (value === '') {
      return "Please enter an email";
    } else if (!emailRegex.test(value)) {
      return "Please enter a valid email"
    }
    return true;
  },
];

const password = ref("");

const passwordRules = [
  (value: string) => {
    if (value === '') {
      return 'Please enter a password';
    }
    return true;
  }
];

const loginForm: Ref<VForm | null> = ref(null);

const subtitle = computed(() => {
  return store.hasNoSite ? 'Select Domain' : `${store.$state.name} Login`
})

function handleSiteSelection(site: Site) {
  loading.value = true;
  const currentLocation = window.location;
  const newUrl = `${currentLocation.protocol}//${site.subdomain}.${currentLocation.host}/login`;
  window.location.href = newUrl;
}

function clearValidation() {
  if (loginForm.value) {
    loginForm.value.resetValidation();
  }
}

async function tryLogin(): Promise<void> {
  loading.value = true;
  return new Promise((resolve) => {
    setTimeout(() => { 
      loading.value = false;
      resolve();
    }, 3000)
  });
}

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
        v-text="subtitle"
        class="login__title mb-2"
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
          <v-form 
            :disabled="loading"
            ref="loginForm"
            validate-on="submit lazy"
            @submit.prevent="tryLogin"
          >
            <v-text-field 
              v-model="email"
              :rules="emailRules"
              class="mb-2"
              type="email"
              label="Email"
              variant="underlined"
              @input="clearValidation"
            />
            <v-text-field 
              v-model="password"
              :rules="passwordRules"
              class="mb-2"
              type="password"
              label="Password"
              variant="underlined"
              @input="clearValidation"
            />
            <v-btn
              :loading="loading"
              variant="outlined"
              type="submit"
              class="mt-2"
              text="Submit"
            ></v-btn>
          </v-form>
        </template>
      </v-card-text>
    </v-card>
  </div>

</template>

<style scoped>

</style>
