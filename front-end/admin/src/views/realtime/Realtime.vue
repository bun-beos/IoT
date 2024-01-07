<template>
  <div id="realtime">
    <Container :pageTitle="'Thời gian thực'">
      <div class="realtime-content">
        <div class="realtime-data">
          <div class="data-mode">
            <DButton
              v-if="currentFormat == 1"
              :bgColor="'white'"
              class="realtime-video"
              @click="onSelectVideo"
              >Video</DButton
            >
            <DButton v-else class="realtime-video" @click="onSelectVideo">Video</DButton>
            <DButton
              v-if="currentFormat == 0"
              :bgColor="'white'"
              class="realtime-image"
              @click="onSelectImage"
              >Ảnh</DButton
            >
            <DButton v-else class="realtime-image" @click="onSelectImage">Ảnh</DButton>
          </div>
          <div style="min-height: 200px;">
            <img v-if="realtimeData.ImageUrl != ''" :src="realtimeData.ImageUrl" alt=""/>
            <div v-else>Có lỗi xảy ra</div>
          </div>
          <div v-if="currentFormat == 1" style="margin-top: 12px;">
            <div class="file-name" style="font-size: 16px">
              Tên file: {{ realtimeData.ImageName }}
            </div>
            <div class="image-time" style="font-size: 16px">
              Thời gian: {{ imageTimeFormated }}
            </div>
          </div>
          <div v-else>
            
          </div>
        </div>

        <div class="realtime-action">
          <div class="door-status" v-if="deviceStatus == 1">
            Trạng thái cửa: {{ (status = newDoorStatus == 1 ? "Mở" : "Đóng") }}
          </div>
          <div class="door-status" v-else>Trạng thái cửa: Không rõ</div>
          <DButton class="open-door" @click="onControlDoor(1)">Mở cửa</DButton>
          <DButton :bgColor="'error'" class="lock-door" @click="onControlDoor(0)"
            >Khóa cửa</DButton
          >
          <div style="margin-top: 30px">
            <DButton
              v-if="currentFormat == 1"
              :bgColor="'primary'"
              class="take-photo"
              @click="realtimeStore.getNewImageAsync"
              >Chụp ảnh mới</DButton
            >
          </div>
        </div>
      </div>
    </Container>
  </div>
</template>

<script setup>
import Container from "../../components/layouts/container/Container.vue";
import { ref, onMounted } from "vue";
import { useRealtimeStore } from "./RealtimeStore";
import { storeToRefs } from "pinia";
const realtimeStore = useRealtimeStore();
const {
  realtimeData,
  imageTimeFormated,
  deviceStatus,
  doorStatus,
  newDoorStatus,
} = storeToRefs(realtimeStore);
const currentFormat = ref(1);
const status = ref("");

onMounted(() => {
  realtimeStore.realtimeData = {
    ImageId: "",
    ImageName: "",
    ImageTime: "",
    ImageUrl: "",
  };
  realtimeStore.deviceStatus = 0;
  realtimeStore.getStatusAsync();
  realtimeStore.getLatestImageAsync();
  realtimeStore.newDoorStatus = realtimeStore.doorStatus;
});

function onSelectImage() {
  currentFormat.value = 1;
  realtimeStore.getLatestImageAsync();
}

function onSelectVideo() {
  currentFormat.value = 0;
  realtimeStore.realtimeData.ImageUrl = "http://192.168.0.113/Videos";
}

function onControlDoor(value) {
  realtimeStore.doorStatus = value;
  realtimeStore.controlDoorAysnc();
}
</script>

<style lang="css" scoped>
@import url(./Realtime.css);
</style>
