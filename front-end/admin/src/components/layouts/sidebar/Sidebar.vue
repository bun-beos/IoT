<template>
  <div id="sidebar">
    <div class="sidebar-account">
      <span class="mdi mdi-account-circle"></span>
      <router-link to="/" class="account-name">User name</router-link> 
    </div>
    <router-link to="/Dashboard" class="sidebar-item">
      <div class="item-icon">
        <span class="mdi mdi-view-dashboard"></span>
      </div>
      <div class="item-title">Dashboard</div>
    </router-link>
    <router-link to="/Realtime" class="sidebar-item">
      <div class="item-icon">
        <span class="mdi mdi-clock-outline"></span>
      </div>
      <div class="item-title">Dữ liệu thời gian thực</div>
    </router-link>
    <router-link to="/History" class="sidebar-item">
      <div class="item-icon">
        <span class="mdi mdi-history"></span>
      </div>
      <div class="item-title">Lịch sử</div>
    </router-link>
  </div>
</template>

<script setup></script>

<style scoped>
#sidebar {
  width: 250px;
  height: 100vh;
  color: #fff;
  background-color: #333;
}

.sidebar-account {
  height: 56px;
  display: flex;
  align-items: center;
  font-size: 20px;
}

.sidebar-account .mdi-account-circle {
  font-size: 40px;
  margin: 8px;
}

.sidebar-account .account-name {
  font-size: 24px;
}

.sidebar-item {
  display: flex;
  align-items: center;
  height: 36px;
  margin: 2px 0;
  cursor: pointer;
}

.sidebar-item:hover {
  background-color: #555;
}

.sidebar-item .item-icon {
  margin-left: 8px;
  margin-right: 6px;
}

.item-icon span {
  font-size: 20px;
}

.item-title {
  font-size: 14px;
}
</style>
