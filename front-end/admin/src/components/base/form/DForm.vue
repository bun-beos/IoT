<template>
  <div class="fixed__container">
    <!-- <w-overlay v-model="showOverlay"> -->
    <div class="form__container">
      <slot name="form_body"></slot>
    </div>
    <!-- </w-overlay> -->
  </div>
</template>

<script setup>
import { ref } from "vue";
const props = defineProps({});
const showOverlay = ref(true);
</script>

<style lang="scss" scoped>
.fixed__container {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  display: flex;
  justify-content: center;
  align-items: center;
  background-color: rgba(0, 0, 0, 0.3);
  z-index: 100;
}

.form__container {
  background-color: #fff;
  padding: 24px;
  border-radius: 4px;
  min-width: 500px;
}
</style>
