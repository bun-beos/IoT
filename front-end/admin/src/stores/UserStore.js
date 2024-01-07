import { defineStore } from "pinia";
import axios from "@/js/axios";
// import {useLocal} from "vue";
import { useNotificationStore } from "@/stores/NotificationStore";
export const useUserStore = defineStore("userStore", {
  state: () => ({
    isLogined: false,
    accessToken: "",
    role: "",
    loginData: {
      UserName: "",
      Password: "",
    },
    notificationStore: useNotificationStore(),
  }),
  getters: {},
  actions: {
    async loginAsync() {
      try {
        this.notificationStore.showLoading();
        const response = await axios.post("Authenticate/login", this.loginData);
        this.isLogined = true;
        this.accessToken = response.data.AccessToken;
        localStorage.setItem("actkn", response.data.AccessToken);
        this.role = response.data.Role;
        this.notificationStore.hideLoading();
      } catch (error) {
        this.notificationStore.hideLoading();
        console.log(error);
      }
    },
  },
});
