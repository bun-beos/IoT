import { defineStore } from "pinia";
import myAxiosESP from "@/js/axios";
import myAxiosBE from "@/js/axiosbe";

export const useRealtimeStore = defineStore("realtimeStore", {
    state: () => ({
        realtimeData: {
            ImageId: "",
            ImageName: "",
            ImageTime: "",
            ImageUrl: "",
        },
        imageTimeFormated: "",
        deviceStatus: 0,
        doorStatus: 0,
        newDoorStatus: 0,
        historyImage: [],
        historyDoorStatus: [],
    }),
    getters: {},
    actions: {
        formatDateTime(date) {
            if (date == null) return ""
            date = new Date(date)

            let dateValue = date.getDate()
            dateValue = dateValue < 10 ? `0${dateValue}` : dateValue

            let monthValue = date.getMonth() + 1
            monthValue = monthValue < 10 ? `0${monthValue}` : monthValue

            let yearValue = date.getFullYear()
            if (100 <= yearValue && yearValue < 1000) yearValue = `0${yearValue}`
            else if (10 <= yearValue && yearValue < 100) yearValue = `00${yearValue}`
            else if (yearValue < 10) yearValue = `000${yearValue}`

            let hourValue = date.getHours()
            hourValue = hourValue < 10 ? `0${hourValue}` : hourValue

            let minValue = date.getMinutes()
            minValue = minValue < 10 ? `0${minValue}` : minValue

            let secValue = date.getSeconds()
            secValue = secValue < 10 ? `0${secValue}` : secValue

            return `${dateValue}/${monthValue}/${yearValue} ${hourValue}:${minValue}:${secValue}`
        },

        async getLatestImageAsync() {
            try {
                const response = await myAxiosBE.get("/ESP32Cams/Latest")
                this.realtimeData = response.data
                this.imageTimeFormated = this.formatDateTime(this.realtimeData.ImageTime)
            } catch (error) {
                console.log(error)
            }
        },

        async getNewImageAsync() {
            try {
                const response = await myAxiosESP.get("/NewImage")
                this.realtimeData = response.data
                this.imageTimeFormated = this.formatDateTime(this.realtimeData.ImageTime)
            } catch (error) {
                console.log(error)
            }
        },

        async controlDoorAysnc() {
            try {
                const response = await myAxiosESP.get(`/Doors?status=${this.doorStatus}`)
                this.newDoorStatus = response.data
            } catch (error) {
                console.log(error)
                this.newDoorStatus = 0
            }
        },

        async getStatusAsync() {
            try {
                const resposne = await myAxiosESP.get("/")
                this.deviceStatus = resposne.data.DeviceStatus
                this.doorStatus = resposne.data.DoorStatus
            } catch (error) {
                console.log(error)
            }
        },

        async getHisotyImageAsync() {
            try {
                const response = await myAxiosBE.get("/ESP32Cams")
                this.historyImage = response.data
                for (const item of this.historyImage) {
                    item.ImageTime = this.formatDateTime(item.ImageTime)
                }
            } catch (error) {
                console.log(error)
            }
        },
        
        async getHisotyDoorStatusAsync() {
            try {
                const response = await myAxiosBE.get("/ESP32Cams/DoorStatus")
                this.historyDoorStatus = response.data
                for (const item of this.historyDoorStatus) {
                    item.DoorStatusTime = this.formatDateTime(item.DoorStatusTime)
                    item.DoorStatusValue = item.DoorStatusValue == "1"? "Mở" : "Đóng"
                }
            } catch (error) {
                console.log(error)
            }
        }
    }
})