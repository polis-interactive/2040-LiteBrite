<script setup lang="ts">
import GenericMessage from '~/components/layout/generic-message.vue';

definePageMeta({
  layout: 'centered'
})
</script>

<template>
  <generic-message>
    <p class="text-subtitle-1 mt-2">
      If you're seeing this, I'm just as suprised as you are...
    </p>
    <p class="mt-2">
      Try removing the "internal guard" bit from you're URL bar. If that doesn't work, contact broose or something...
    </p>
  </generic-message>
</template>

<style scoped>
</style>

