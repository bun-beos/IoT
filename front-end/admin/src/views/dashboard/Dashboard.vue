<template>
  <div id="dashboard">
    <Container :pageTitle="'Dashboard'">
      <div class="dashboard-content overflow-y-scroll">
        <div class="status flex">
          <div class="device-status mr-[32px]" style="margin-bottom: 12px">
            <div>
              <div class="title">Trạng thái thiết bị:</div>
              <div class="content" v-if="deviceStatus == 1">Đang hoạt động</div>
              <div class="content" v-else>Tạm dừng hoạt động</div>
            </div>
            <span
              class="mdi mdi-camera-wireless-outline"
              :class="{ stopped: deviceStatus != 1 }"
            ></span>
          </div>
          <div class="door-status">
            <div>
              <div class="title">Trạng thái cửa:</div>
              <div class="content" v-if="doorStatus == 0 && deviceStatus == 1">
                Đang đóng
              </div>
              <div class="content" v-if="doorStatus == 1 && deviceStatus == 1">
                Đang mở
              </div>
              <div class="content" v-if="deviceStatus != 1">Không rõ</div>
            </div>
            <span
              class="mdi mdi-door-closed-lock"
              v-if="doorStatus == 0 && deviceStatus == 1"
            ></span>
            <span
              class="mdi mdi-door-open"
              v-if="doorStatus == 1 && deviceStatus == 1"
            ></span>
            <span
              class="mdi mdi-door-closed-cancel"
              v-if="deviceStatus != 1"
            ></span>
          </div>
        </div>

        <div class="chart pt-[16px]" v-if="showChart">
          <div class="flex items-center justify-between">
            <h1 class="my-[16px]">
              Biểu đồ hoạt động tháng {{ new Date().getMonth() + 1 }}
            </h1>

            <div class="flex mr-[200px]">
              <div class="font-bold mr-[24px]">Chú giải</div>
              <div class="flex">
                <div class="w-[16px] h-[16px] bg-[#90e0ef] mr-[6px]"></div>
                <div>Số lần mở cửa</div>
              </div>
              <div class="flex ml-[16px]">
                <div class="w-[16px] h-[16px] bg-[#0096c7] mr-[6px]"></div>
                <div>Số lần chụp ảnh</div>
              </div>
            </div>
          </div>

          <div class="flex items-start relative">
            <div class="absolute">Số lần</div>
            <div class="flex items-end pt-[8px]">
              <Chart
                :size="{ width: 1100, height: 420 }"
                :data="data"
                :margin="margin"
                :direction="direction"
                :axis="axis"
              >
                <template #layers>
                  <Grid strokeDasharray="(2, 2)" />
                  <Bar
                    :dataKeys="['Date', 'OpenDoor']"
                    :barStyle="{ fill: '#90e0ef' }"
                  />
                  <Bar
                    :dataKeys="['Date', 'TakePhoto']"
                    :barStyle="{ fill: '#0096c7' }"
                  />
                  <!-- <Bar
                    :dataKeys="['name', 'inc']"
                    :barStyle="{ fill: '#48cae4' }"
                  /> -->
                  <Marker
                    :value="1000"
                    label="Avg."
                    color="#e76f51"
                    strokeWidth="2"
                    strokeDasharray="6 6"
                  />
                </template>
                <template #widgets>
                  <Tooltip
                    borderColor="#48CAE4"
                    :config="{
                      OpenDoor: { color: '#90e0ef' },
                      TakePhoto: { color: '#0096c7' },
                    }"
                  />
                </template>
              </Chart>
              <div>Ngày</div>
            </div>
          </div>
        </div>

        <div class="chart pt-[16px]" v-if="showChart">
          <div class="flex items-center justify-between">
            <h1 class="my-[16px]">Biểu đồ hoạt động ngày hôm nay</h1>

            <div class="flex mr-[200px]">
              <div class="font-bold mr-[24px]">Chú giải</div>
              <div class="flex">
                <div class="w-[16px] h-[16px] bg-[#90e0ef] mr-[6px]"></div>
                <div>Số lần mở cửa</div>
              </div>
              <div class="flex ml-[16px]">
                <div class="w-[16px] h-[16px] bg-[#0096c7] mr-[6px]"></div>
                <div>Số lần chụp ảnh</div>
              </div>
            </div>
          </div>

          <div class="flex items-start relative">
            <div class="absolute">Số lần</div>
            <div class="flex items-end pt-[8px]">
              <Chart
                :size="{ width: 1100, height: 420 }"
                :data="arrayAcitvityTodayTimes"
                :margin="margin"
                :direction="direction"
                :axis="axis"
              >
                <template #layers>
                  <Grid strokeDasharray="(2, 2)" />
                  <Bar
                    :dataKeys="['Hour', 'OpenDoor']"
                    :barStyle="{ fill: '#90e0ef' }"
                  />
                  <Bar
                    :dataKeys="['Hour', 'TakePhoto']"
                    :barStyle="{ fill: '#0096c7' }"
                  />
                  <!-- <Bar
                :dataKeys="['name', 'inc']"
                :barStyle="{ fill: '#48cae4' }"
              /> -->
                  <Marker
                    :value="1000"
                    label="Avg."
                    color="#e76f51"
                    strokeWidth="2"
                    strokeDasharray="6 6"
                  />
                </template>

                <template #widgets>
                  <Tooltip
                    borderColor="#48CAE4"
                    :config="{
                      OpenDoor: { color: '#90e0ef' },
                      TakePhoto: { color: '#0096c7' },
                    }"
                  />
                </template>
              </Chart>
              <div>Giờ</div>
            </div>
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
import { Chart, Grid, Bar, Marker, Tooltip } from "vue3-charts";

const realtimeStore = useRealtimeStore();

const {
  doorStatus,
  deviceStatus,
  arrayAcitityDoorTimes,
  arrayAcitvityTodayTimes,
} = storeToRefs(realtimeStore);
const data = ref(arrayAcitityDoorTimes);
const showChart = ref(false);
const direction = ref("horizontal");
const margin = ref({
  left: 0,
  top: 20,
  right: 20,
  bottom: 0,
});

const axis = {
  primary: {
    type: "band",
  },
  secondary: {
    domain: ["dataMin", "dataMax + 5"],
    type: "linear",
    ticks: 8,
  },
};
onMounted(async () => {
  realtimeStore.deviceStatus = 0;
  realtimeStore.doorStatus = 0;
  realtimeStore.getStatusAsync();
  realtimeStore.initActivityArrayTimes();
  await realtimeStore.getHisotyDoorStatusAsync();
  await realtimeStore.getHisotyImageAsync();
  showChart.value = true;
  console.log(data.value);
});
</script>

<style lang="css" scoped>
@import url(./Dashboard.css);
</style>
