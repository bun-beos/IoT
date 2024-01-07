<template>
  <div id="dashboard">
    <Container :pageTitle="'Dashboard'">
      <div class="dashboard-content">
        <div class="status">
          <div class="device-status" style="margin-bottom: 12px">
            <div>
              <div class="title">Trạng thái thiết bị:</div>
              <div class="content" v-if="deviceStatus == 1">Đang hoạt động</div>
              <div class="content" v-else>Tạm dừng hoạt động</div>
            </div>
            <span class="mdi mdi-camera-wireless-outline" :class="{'stopped': deviceStatus != 1}"></span>
          </div>
          <div class="door-status">
            <div>
              <div class="title">Trạng thái cửa:</div>
              <div class="content" v-if="doorStatus == 0 && deviceStatus == 1">Đang đóng</div>
              <div class="content" v-if="doorStatus == 1 && deviceStatus == 1">Đang mở</div>
              <div class="content" v-if="deviceStatus != 1">Không rõ</div>
            </div>
            <span class="mdi mdi-door-closed-lock" v-if="doorStatus == 0 && deviceStatus == 1"></span>
            <span class="mdi mdi-door-open" v-if="doorStatus == 1 && deviceStatus == 1"></span>
            <span class="mdi mdi-door-closed-cancel" v-if="deviceStatus != 1"></span>
          </div>
        </div>
      </div>
    </Container>
  </div>
</template>

<script setup>
import Container from "../../components/layouts/container/Container.vue";
import { ref, onMounted } from "vue";
import { storeToRefs } from "pinia";
import { useRealtimeStore } from "../realtime/RealtimeStore";

const realtimeStore = useRealtimeStore()
const {
  doorStatus,
  deviceStatus,
} = storeToRefs(realtimeStore)

onMounted(() => {
  realtimeStore.deviceStatus = 0
  realtimeStore.doorStatus = 0
  realtimeStore.getStatusAsync()
})
</script>

<style lang="css" scoped>
@import url(./Dashboard.css);
</style>
