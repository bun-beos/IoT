<template>
  <div class="toast-message-box" v-show="showToastMessage">
    <div class="toast-message-icon">
      <w-icon :style="{ color: toastMesContent?.IconColor }">
        {{ toastMesContent?.Icon }}
      </w-icon>
    </div>
    <div class="toast-message-desc">
      <span class="toast-message-status" :class="toastMesContent?.TextClass">
        {{ toastMesContent?.Status }}
      </span>
      {{ toastMesContent?.Desc }}
    </div>
    <div class="toast-message-closebox">
      <div class="toast-message-option">
        <a href="">{{ toastMesContent?.OptionAction }}</a>
      </div>
      <div class="toast-message-close" @click="clickClose">
        <w-icon>mdi mdi-close-thick</w-icon>
      </div>
    </div>
  </div>
</template>
<script setup>
import { ref } from "vue";
const props = defineProps({
  onClose: {},
  toastMesContent: {
    type: Object,
  },
});

const showToastMessage = ref(true);

/**
 * Đóng toast message
 * Created by: nkdang 2/11/2023
 */
function clickClose() {
  showToastMessage.value = false;
}
</script>
<style scoped>
@import url(./toastmessage.css);

.text-50b83c {
  color: #50b83c;
  font-weight: bold;
}

.text-DE3618 {
  color: #de3618;
}

.text-F49342 {
  color: #f49342;
}

.text-error {
  color: rgb(244, 147, 66);
}

.icon__miniclose {
  background-position: -1177px -362px;
  width: 13px;
  height: 12px;
}

.ti-close {
  color: #000;
}
.icon__error {
  background-position: -1136px -467px;
  width: 24px;
  height: 23px;
}

.icon__warning {
  background-position: -593px -145px;
  width: 22px;
  height: 23px;
}
</style>
