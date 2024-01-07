<template>
  <div class="container--fixed">
    <div class="dialog__cont">
      <div class="dialog__titlebox">
        <div class="dialog__title">
          <h1>{{ dialogContent?.Title }}</h1>
        </div>
        <div class="dialog__closebox" @click="dialogStore.hideDialog()">
          <div class="icons"></div>
        </div>
      </div>
      <div class="dialog__content">
        <div class="dialog__icon-box">
          <div class="icons" :class="dialogContent?.Icon"></div>
        </div>
        <div class="dialog__mes" v-html="dialogContent?.Message"></div>
      </div>

      <div class="dialog__button-box">
        <div>
          <DButton>Đồng ý</DButton>
          <DButton bg-color="white">Hủy</DButton>
        </div>
        <div
          class="dialog__button--accept flex"
          @click="dialogStore.clickAccept()"
        ></div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { useNotificationStore } from "@/stores/NotificationStore";
import { storeToRefs } from "pinia";
const dialogStore = useNotificationStore();
const { dialogContent } = storeToRefs(dialogStore);
</script>

<style scoped>
.dialog__container {
  position: fixed;
  left: 0;
  right: 0;
  top: 0;
  bottom: 0;
  display: flex;
  justify-content: center;
  align-items: center;
  background-color: #ccc;
}

.dialog__box {
  background-color: #fff;
  border-radius: 4px;
  box-shadow: 0 2px 3px #ccc;
}
.container--fixed {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: rgba(0, 0, 0, 0.4);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 6;
}

.dialog__cont {
  width: 450px;
  min-height: 173px;
  background-color: #fff;
  border-radius: 4px;
  padding: 24px;
}

.dialog__titlebox {
  display: flex;
  justify-content: space-between;
  margin-bottom: 22px;
}

.dialog__title h1 {
  font-weight: bold !important;
  font-size: 20px;
}

.dialog__mes {
  width: 80%;
  /* white-space: pre;
  word-wrap: break-word; */
}

.dialog__closebox {
  display: flex;
  align-items: center;
  cursor: pointer;
}

.icon__miniclose {
  background-position: -1177px -362px;
  width: 13px;
  height: 12px;
}

.dialog__content {
  display: flex;
}

.dialog__icon-box {
  display: flex;
  align-items: center;
  margin-right: 16px;
}

.dialog__button-box {
  display: flex;
  margin-top: 24px;
  justify-content: space-between;
}

.dialog-button__cancel {
  margin-right: 8px;
}

.icon__bigwarning {
  background-position: -598px -463px;
  width: 36px;
  height: 37px;
}
</style>
