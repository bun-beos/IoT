<template>
  <div class="toast__group">
    <ToastMessage
      v-for="(toastMessageContent, index) in toastContents"
      :key="index"
      :toastMesContent="toastMessageContent"
    ></ToastMessage>
  </div>
</template>

<script setup>
import ToastMessage from "./ToastMessage.vue";
import { useNotificationStore } from "@/stores/NotificationStore";
import { storeToRefs } from "pinia";

const toastMessageStore = useNotificationStore();
const { toastContents } = storeToRefs(toastMessageStore);
</script>
<style>
.toast__group {
  position: fixed;
  right: 20px;
  bottom: 10px;
  min-width: 450px;
  max-width: 650px;
  min-height: 1px;
  display: flex;
  flex-direction: column;

  z-index: 999;
}
</style>
