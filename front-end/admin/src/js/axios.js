import axios from "axios";
const myAxiosESP = axios.create({
  baseURL: "http://192.168.164.29",
});
export default myAxiosESP;
