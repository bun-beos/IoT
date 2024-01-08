<template>
  <div id="history">
    <Container :pageTitle="'Lịch sử'">
      <div class="history-content">
        <div class="history-select">
          <DButton
            v-if="historySelect"
            :bgColor="'white'"
            class="history-image"
            @click="onHistorySelect"
            >Ảnh đã chụp</DButton
          >
          <DButton v-else class="history-image" @click="onHistorySelect"
            >Ảnh đã chụp</DButton
          >
          <DButton
            v-if="!historySelect"
            :bgColor="'white'"
            class="history-door"
            @click="onHistorySelect"
            >Trạng thái cửa</DButton
          >
          <DButton v-else class="history-door" @click="onHistorySelect"
            >Trạng thái cửa</DButton
          >
        </div>

        <div class="history-table" style="height: calc(100% - 80px)">
          <div style="height: 50px" v-if="!historySelect">
            <DButton @click="onSelectTableFormat" v-if="tableFormat" :bgColor="'white'"
              >Danh sách</DButton
            >
            <DButton @click="onSelectTableFormat" v-else>Danh sách</DButton>
            <DButton @click="onSelectTableFormat" v-if="!tableFormat" :bgColor="'white'"
              >Lưới</DButton
            >
            <DButton @click="onSelectTableFormat" v-else>Lưới</DButton>
          </div>
          <div style="height: calc(100% - 50px); overflow-y: auto">
            <table v-if="!historySelect && !tableFormat" class="list-table">
              <thead>
                <tr>
                  <th>Tên ảnh</th>
                  <th>Thời gian</th>
                  <th>Link ảnh</th>
                </tr>
              </thead>
              <tbody>
                <tr v-for="(item, index) in historyImage" :key="index">
                  <td>{{ item.ImageName }}</td>
                  <td>{{ item.ImageTime }}</td>
                  <td>
                    <a :href="item.ImageUrl" target="_blank">{{ item.ImageUrl }}</a>
                  </td>
                </tr>
              </tbody>
            </table>

            <table v-if="!historySelect && tableFormat" class="grid-table">
              <tr
                v-for="(item, index) in Math.ceil(historyImage.length / 3)"
                :key="index"
              >
                <td v-for="idx in 3">
                  <div v-if="historyImage[index * 3 + idx - 1] != undefined">
                    <div style="min-height: 200px">
                      <img :src="historyImage[index * 3 + idx - 1].ImageUrl" alt="" />
                    </div>
                    <div>Tên: {{ historyImage[index * 3 + idx - 1].ImageName }}</div>
                    <div>
                      Thời gian: {{ historyImage[index * 3 + idx - 1].ImageTime }}
                    </div>
                  </div>
                </td>
              </tr>
            </table>

            <table v-if="historySelect" class="list-table">
              <thead>
                <tr>
                  <th>Thời gian</th>
                  <th>Trạng thái cửa</th>
                </tr>
              </thead>
              <tbody>
                <tr v-for="(item, index) in historyDoorStatus" :key="index">
                  <td v-if="item.DoorStatusTime.includes('T')">{{ realtimeStore.formatDateTime(item.ImageTime) }}</td>
                  <td>
                    {{ item.DoorStatusValue }}
                  </td>
                </tr>
              </tbody>
            </table>
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
const realtimeStore = useRealtimeStore();
const { historyImage, historyDoorStatus } = storeToRefs(realtimeStore);
const historySelect = ref(false);
const tableFormat = ref(false);

onMounted(() => {
  realtimeStore.getHisotyImageAsync();
});

function onHistorySelect() {
  historySelect.value = !historySelect.value;
  if (!historySelect) {
    realtimeStore.getHisotyImageAsync();
  } else {
    realtimeStore.getHisotyDoorStatusAsync();
  }
}

function onSelectTableFormat() {
  tableFormat.value = !tableFormat.value;
}
</script>

<style lang="css" scoped>
@import url(./History.css);
</style>
