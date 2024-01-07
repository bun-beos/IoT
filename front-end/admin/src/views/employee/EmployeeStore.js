import { defineStore } from "pinia";
import axios from "@/js/axios";
import { useNotificationStore } from "@/stores/NotificationStore";
import { useHelperStore } from "@/stores/HelperStore";
import { resource } from "./EmployeeResource";
import { useUserStore } from "@/stores/UserStore";
export const useEmployeeStore = defineStore("employeeStore", {
  state: () => ({
    headers: [
      {
        label: "checkBox",
        key: "EmployeeId",
        sticky: true,
        width: "50px",
        sortable: false,
      },
      {
        label: "Mã nhân viên",
        key: "EmployeeCode",
        width: "200px",
        reasizable: true,
      },
      { label: "Họ và tên", key: "FullName", width: "200px" },
      { label: "Ngày sinh", key: "DateOfBirth", width: "220px" },
      { label: "Giới tính", key: "Gender", width: "100px" },
      { label: "Đơn vị", key: "DepartmentName", width: "150px" },
      {
        label: "Chức danh",
        key: "PositionName",
        width: "250px",
      },
      { label: "Số tài khoản", key: "BankAccount", width: "250px" },
      { label: "Tên ngân hàng", key: "BankName", width: "150px" },
      { label: "Chi nhánh ngân hàng", key: "BankBranch", width: "210px" },
      {
        label: "Chức năng",
        key: "functionBox",
        sticky: true,
        width: "100px",
        sortable: false,
      },
    ],

    employeesData: [],
    numEmployees: 0,
    page: 1,
    pageSize: 50,
    start: 1,
    end: 50,
    helperStore: useHelperStore(),
    notificationStore: useNotificationStore(),
    userStore: useUserStore(),
    resourceLanguage: resource[useHelperStore().languageCode],

    // Các biến phục vụ việc chọn nhân viên trên bảng
    selectedEmployeeIdsObject: {},
    selectedEmployeeIds: [],
    isBatchProcess: false,
    isAllPageProcess: false,
    numCurrentPageEmployeeSelected: 0,

    // Thông tin tìm kiếm nhân viên
    employeeProperty: "",

    // Thông tin form data nhân viên
    employeeFormData: {},
  }),
  getters: {},
  actions: {
    async getEmployeeDataAsync() {
      try {
        this.notificationStore.showLoading();
        const response = await axios.get(
          `Employees?page=${this.page}&pageSize=${this.pageSize}&employeeProperty=${this.employeeProperty}`,
          {
            headers: {
              Authorization: `Bearer ${this.userStore.accessToken}`,
            },
          }
        );
        this.employeesData = response.data.data;
        this.numEmployees = response.data.countEmployees;
        this.start = (this.page - 1) * this.pageSize + 1;
        this.end = this.start + response.data.data.length - 1;
        this.numPages = Math.ceil(this.numEmployees / this.pageSize);
        this.numCurrentPageEmployeeSelected = 0;
        this.notificationStore.hideLoading();
      } catch (error) {
        this.notificationStore.hideLoading();
        this.start = 0;
        this.end = 0;
        this.notificationStore.showToastMessage(
          this.resourceLanguage.ToastMessage.CannotGetData
        );
      }
    },

    /**
     * Hàm thay đổi số bản ghi trong trang
     * @param {Number} newPageSize
     *
     * Created by: nkdang 2/11/2023
     */
    async changePageSize(newPageSize) {
      this.pageSize = newPageSize;
      if ((this.page - 1) * this.pageSize >= this.numLeaveDaysRequest) {
        this.page = this.page - 1;
      }
      // console.log(this.pageSize);
      await this.getEmployeeDataAsync();
      // console.log(this.processedData);
    },

    /**
     * Hàm chuyển sang trang tiếp theo
     * Created by: nkmdang 14/11/2023
     */
    async goToNextPageAsync() {
      if (this.page < this.numPages) {
        this.page = this.page + 1;
        await this.getEmployeeDataAsync();
      }
    },

    /**
     * Hàm quay về trang trước
     */
    async goToPrevPageAsync() {
      if (this.page > 1) {
        this.page = this.page - 1;
        await this.getEmployeeDataAsync();
      }
    },

    /**
     * Hàm tích chọn nhân viên trong bảng
     * @param {Guid} employeeId
     * Created by: nkdang 12/12/2023
     */
    selectEmployee(employeeId) {
      this.selectedEmployees.employeeId = true;
    },

    /**
     * Hàm nhận file excel từ backend
     * @param {Int} page
     * @param {Int} pageSize
     * @param {String} employeeProperty
     *
     * Created By: nkmdang 10/10/2023
     */
    async exportExcelCurrentPage(page, pageSize, employeeProperty, aRef) {
      // this.employeePropertyExcel = this.employeeProperty;
      try {
        this.notificationStore.showLoading();
        const response = await axios.get(
          `Employees/EmployeesExcel?page=${page}&pageSize=${pageSize}`,
          { responseType: "blob" }
        );
        // Tạo một Blob từ dữ liệu trả về từ API
        const blob = new Blob([response.data]);

        // Tạo URL cho Blob
        const url = window.URL.createObjectURL(blob);

        // Lấy thẻ <a> tải xuống và đặt href là URL của Blob
        aRef.href = url;

        // Đặt tên tệp Excel mà bạn muốn khi người dùng tải về
        aRef.download = "Danh_sach_nhan_vien.xlsx";

        // Simulate a click to trigger the download
        aRef.click();

        // Giải phóng URL để tránh rò rỉ bộ nhớ
        window.URL.revokeObjectURL(url);
        // console.log(response);
        this.notificationStore.hideLoading();
      } catch (error) {
        this.notificationStore.hideLoading();
        console.log(error);
        this.notificationStore.showToastMessage(
          this.resourceLanguage.ToastMessage.CannotExportExcel
        );
      }
    },

    // Xử lý việc chọn các nhân viên để thực hiện hàng loạt
    /**
     * Hàm thêm id của nhân viên được chọn vào object các id của các nhân viên được chọn
     * @param {Guid (String)} employeeId
     */
    selectOneEmployee(employeeId) {
      if (!this.selectedEmployeeIdsObject[employeeId]) {
        this.selectedEmployeeIdsObject[employeeId] = false;
        this.numCurrentPageEmployeeSelected -= 1;
      } else {
        this.numCurrentPageEmployeeSelected += 1;
        this.selectedEmployeeIdsObject[employeeId] = true;
      }
      this.handleStateProcess();
    },

    /**
     * Hàm xử lý trạng thái trong bảng là đang xử lý theo lô (không tất cả) hay là đang xử lý tất cả
     * Created by: nkdang 14/12/2023
     */
    handleStateProcess() {
      if (this.numCurrentPageEmployeeSelected < 1) {
        this.isAllPageProcess = false;
        this.isBatchProcess = false;
      } else if (
        this.numCurrentPageEmployeeSelected > 1 &&
        this.numCurrentPageEmployeeSelected < this.pageSize
      ) {
        this.isBatchProcess = true;
      } else {
        this.isBatchProcess = false;
        this.isAllPageProcess = true;
      }
    },

    /**
     * Hàm xóa một nhân viên theo Id
     * @param {Guid - String} employeeId
     */
    async deleteOneEmployee(employeeId) {
      try {
        this.notificationStore.showLoading();
        const response = await axios.delete(`/Employees/${employeeId}`);
        this.notificationStore.hideLoading();
      } catch (error) {
        this.notificationStore.hideLoading();
        this.start = 0;
        this.end = 0;
        this.notificationStore.showToastMessage(
          this.resourceLanguage.ToastMessage.DeleteEmployeeFailed
        );
      }
    },
  },
});
