<script setup lang="ts">
import { useAuth0 } from "~/composables/useAuth"

definePageMeta({
    layout: 'centered'
})

const startLogin = async () => {
    const auth0 = await useAuth0();
    await auth0.value.loginWithRedirect();
}
</script>

<template>
    <v-card
      min-width="400px"
      variant="outlined"
    >
      <v-card-title 
        v-text="'Light Controller'"
      />
      <v-card-subtitle
        v-text="'Logout'"
        class="mb-2"
      />
      <v-card-text class="text-center logout__text">
        <p>
          Succesful logout
        </p>
        <p class="mt-1">
          Click below to get back in there!
        </p>
        <v-btn 
            class="mt-6"
            color="secondary"
            variant="tonal"
            @click="startLogin()"
        >
          Login
        </v-btn>
      </v-card-text>
    </v-card>
</template>

<style scoped>
.logout__text {
  font-size: 1rem;
}

</style>
