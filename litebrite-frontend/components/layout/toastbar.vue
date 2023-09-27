<script setup lang="ts">
import { storeToRefs } from 'pinia';

import { useToastStore } from '~/stores/toast'

const toastStore = useToastStore();
const { toastQueue } = storeToRefs(toastStore);

const removeToast = async (id: number) => {
  toastStore.popElement(id);
} 
</script>

<template>
  <VSnackbar
    v-for="(toast) in toastQueue"
    :key="toast.id"
    :model-value="toast.isShow"
    location="bottom end"
    variant="elevated"
    :color="toast.color"
    :timeout="toast.timeout"
    class="mb-2 mr-2"
    :style="{ bottom: `${toast.position}px`, right: '12px' }"
    @update:model-value="removeToast(toast.id)"
  >
    {{ toast.message }}
    <v-btn
      color="white"
      variant="outlined"
      class="ml-2"
      size="small"
      @click="removeToast(toast.id)"
    >
      Close
    </v-btn>
  </VSnackbar>
</template>