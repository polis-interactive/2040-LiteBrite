<script setup lang="ts">
</script>

<template>
  <div class="message__wrapper">
    <div class="message__body">
      <div class="message__side">
        <v-img
          :aspect-ratio="1"
          class="message__side__icon"
          src="/images/site/polis_icon.png" 
        />
      </div>
      <div class="message__main ml-4">
        <h2 class="text-h4">Polis Light Controller</h2>
        <slot />
      </div>
    </div>
  </div>
</template>

<style scoped>
.message__wrapper {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 100%
}
.message__body {
  display: flex;
  flex-direction: row;
  justify-content: center;
  max-width: 540px;
}

.message__side__icon {
  width: 128px;
}
</style>
