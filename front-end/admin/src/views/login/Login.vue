<template>
  <div class="container--fixed">
    <div class="img__box--santa-claus">
      <img src="../../assets/imgs/santa_claus.png" alt="" />
    </div>
    <div class="img__box--pine-tree">
      <img src="../../assets/imgs/pine_tree.png" alt="" />
    </div>
    <div class="login__box">
      <div class="flex pt-[24px] w-[700px] items-center">
        <div class="w-[300px] pr-[30px]">
          <img src="../../assets/imgs/img-01.png" alt="" />
        </div>
        <div class="w-[50%] h-[100%] pl-[30px]">
          <div class="mb-[36px]">
            <h1 class="text-center text-[30px]">Đăng nhập</h1>
          </div>
          <div>
            <DInput
              label="Tên đăng nhập"
              v-model="loginData.UserName"
              :focus="true"
            ></DInput>
          </div>
          <div class="mt-[16px]">
            <DInput
              label="Mật khẩu"
              type="password"
              v-model="loginData.Password"
            ></DInput>
          </div>

          <div class="h-[36px] w-[100%]"></div>

          <div class="flex justify-center">
            <w-button
              bg-color="success"
              height="36px"
              @click="login"
              width="100%"
              class="!rounded-[20px]"
            >
              Đăng nhập
            </w-button>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from "vue";
import { useRouter, useRoute } from "vue-router";
import { useUserStore } from "@/stores/UserStore";
import { storeToRefs } from "pinia";

const userStore = useUserStore();
const { loginData } = storeToRefs(userStore);
const router = useRouter();

async function login() {
  await userStore.loginAsync();
  if (userStore.isLogined == true) {
    router.push("/");
  }
}
</script>

<style lang="scss" scoped>
.container--fixed {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  display: flex;
  justify-content: center;
  align-items: center;
  background-color: #dbe2e3;
  // background-image: url(../../assets/imgs/sun-removebg.png);
}

.login__box {
  border-radius: 10px;
  min-width: 400px;
  min-height: 400px;
  background-color: #fff;
  padding: 24px;
}

.img__box--santa-claus {
  position: fixed;
  bottom: 0;
  left: 0;
}

.img__box--pine-tree {
  position: fixed;
  bottom: 0;
  right: 0;
}
</style>
