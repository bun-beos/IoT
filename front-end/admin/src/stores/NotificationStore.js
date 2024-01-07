import { defineStore } from "pinia";

export const useNotificationStore = defineStore("notificationStore", {
  state: () => ({
    isShowDialog: false,
    isLoading: false,
    selectedEnum: -1,
    dialogContent: {
      Title: "",
      Message: "",
      Icon: "",
    },
    acceptButtonThemeClass: "",
    toastContents: [],
  }),
  getters: {
    getSelection() {
      return this.selectedEnum;
    },
  },
  actions: {
    showLoading() {
      this.isLoading = true;
    },

    hideLoading() {
      this.isLoading = false;
    },

    /**
     * Hàm truyền nội dung vào thông báo và mở thông báo Dialog
     * @param {Object} dialogContent
     * Created by: nkmdang 22/11/2023
     */
    showDialog(dialogContent) {
      this.dialogContent = dialogContent;
      this.isShowDialog = true;
    },

    /**
     * Hàm đóng thông báo Dialog
     * @param {Object} dialogContent
     * Created by: nkmdang 22/11/2023
     */
    hideDialog() {
      this.isShowDialog = false;
    },
    /**
     * Hàm xử lý khi nhấn nút cancel trên dialog
     * Created by: nkmdang 22/11/2023
     */
    clickCancel() {
      this.isShowDialog = false;
      this.selectedEnum = 0;
    },

    /**
     * Hàm xử lý khi nhấn nút not trên dialog
     * Created by: nkmdang 22/11/2023
     */
    clickNot() {
      this.isShowDialog = false;
      this.selectedEnum = 1;
    },
    /**
     * Hàm xử lý khi nhấn accept trên dialog
     * Created by: nkmdang 22/11/2023
     */
    clickAccept() {
      this.isShowDialog = false;
      this.selectedEnum = 2;
    },

    /**
     * Hàm thêm ToastMessageContent vào ToastMessageGroup để mở ToastMessage
     * @param {Object} toastMessageContent
     * Created by: 22/11/2023
     */
    showToastMessage(toastMessageContent) {
      this.toastContents.push(toastMessageContent);
      // console.log(this.toastContents);
      setTimeout(() => this.clearFirstToastMessage(), 5000);
    },

    /**
     * Hàm xóa đi ToastMessage đầu tiên
     * Created by: nkdang 22/11/2023
     */
    clearFirstToastMessage() {
      if (this.toastContents.length > 0) {
        this.toastContents.shift();
      }
    },
  },
});
