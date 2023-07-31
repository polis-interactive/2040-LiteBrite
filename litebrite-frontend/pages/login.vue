<script setup lang="ts">

import { useSiteStore } from '~/stores/site';
import { useUserStore } from '~/stores/user';
import { Site, AvaliableHosts } from '~/lib/domain/sites'
import { TryGetUserFromObject } from '~/lib/domain/user';
import type { VForm } from 'vuetify/components/index'

const siteStore = useSiteStore();
const userStore = useUserStore();

const loading = ref(false);

const validationState = ref(null);
const serverIssue = ref(false);
const invalidLogin = ref(false);

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
    if (serverIssue.value) {
      return "Server issue; please contact bruce...";
    }
    if (invalidLogin.value) {
      return `Invalid Login for ${siteStore.$state.name}`;
    }
    if (value === '') {
      return 'Please enter a password';
    }
    return true;
  }
];

const loginForm: Ref<VForm | null> = ref(null);

const subtitle = computed(() => {
  return siteStore.hasNoSite ? 'Select Domain' : `${siteStore.$state.name} Login`
})

function handleSiteSelection(site: Site) {
  loading.value = true;
  const currentLocation = window.location;
  const newUrl = `${currentLocation.protocol}//${site.subdomain}.${currentLocation.host}/login`;
  window.location.href = newUrl;
}

function clearValidation() {
  loginForm.value?.resetValidation();
  invalidLogin.value = false;
  serverIssue.value = false;
}

async function tryLogin(): Promise<void> {
  loading.value = true;
  const { valid } = await loginForm.value?.validate() ?? { valid: false };
  if (!valid) {
    loading.value = false;
    return;
  }
  const { data, error } = await useBaseUrlFetch('/api/auth/login', {
    method: 'post',
    body: {
      email: email.value, password: password.value, site_id: siteStore.$state.id
    },
    onRequestError() {
      serverIssue.value = true;
      loginForm.value?.validate();
    },
    onResponseError() {
      invalidLogin.value = true;
      loginForm.value?.validate();
    }
  });
  if (!error.value && data.value) {
    const user = TryGetUserFromObject(data.value);
    if (user === null) {
      serverIssue.value = true;
    } else {
      userStore.setUser(user);
      navigateTo("/");
    }
  }
  loading.value = false;
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
        <template v-if="siteStore.hasNoSite">
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
            v-model:model-value="validationState"
            :disabled="loading"
            ref="loginForm"
            validate-on="submit lazy"
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
              class="mt-2"
              text="Submit"
              @click="tryLogin"
            ></v-btn>
          </v-form>
        </template>
      </v-card-text>
    </v-card>
  </div>

</template>

<style scoped>

</style>
