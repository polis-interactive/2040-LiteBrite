<script setup lang="ts">
</script>

<template>
  <div class="page-load__wrapper">
    <div class="page-load__body">
      <v-progress-circular
        :size="60"
        :width="8"
        indeterminate
        color="teal"
        class="mb-2"
      />
      <div class="page-load__text text-h6">
        <slot />
      </div>
    </div>
  </div>
</template>

<style scoped>
.page-load__wrapper {
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  height: 100%
}
.page-load__body {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}
</style>
