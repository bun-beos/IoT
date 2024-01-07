<template>
  <div id="base-layout">
    <Sidebar />
    <router-view></router-view>
    <Progress v-show="isLoading"></Progress>
  </div>
</template>

<script setup>
import { storeToRefs } from "pinia";
import Sidebar from "../sidebar/sidebar.vue";
import Progress from "../../base/progress/Progress.vue";
import { useNotificationStore } from "../../../stores/NotificationStore";

const notificationStore = useNotificationStore();
const { isLoading } = storeToRefs(notificationStore);
</script>

<style lang="css" scoped>
#base-layout {
  display: flex;
}
</style>
