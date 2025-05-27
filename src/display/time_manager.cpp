#include "time_manager.h"

namespace TimeManager {

struct tm timeinfo;
TaskHandle_t timeTaskHandle = NULL;
TaskHandle_t tickTaskHandle = NULL;
SemaphoreHandle_t timeInfoMutex = NULL;
volatile bool updateRequested = false; // 添加更新标志

// 合并所有时间相关操作到一个任务中
void timeManagerTask(void *parameter) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);
  int syncCounter = 0;

  while (true) {
    if (xSemaphoreTake(timeInfoMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      // 检查是否需要NTP更新
      if (updateRequested) {
        struct tm tempTimeInfo;
        if (getLocalTime(&tempTimeInfo)) {
          memcpy(&timeinfo, &tempTimeInfo, sizeof(struct tm));
          syncCounter = 0; // 重置计数器
        }
        updateRequested = false;
      } else {
        // 正常的秒更新
        timeinfo.tm_sec++;
        if (timeinfo.tm_sec >= 60) {
          timeinfo.tm_sec = 0;
          timeinfo.tm_min++;
          if (timeinfo.tm_min >= 60) {
            timeinfo.tm_min = 0;
            timeinfo.tm_hour++;
            if (timeinfo.tm_hour >= 24) {
              timeinfo.tm_hour = 0;
              timeinfo.tm_mday++;
            }
          }
        }
      }
      xSemaphoreGive(timeInfoMutex);

      // 每60秒请求一次NTP同步
      syncCounter++;
      if (syncCounter >= 60) {
        updateRequested = true;
        syncCounter = 0;
      }
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void setup() {
  timeInfoMutex = xSemaphoreCreateMutex();

  // 配置 NTP 服务器
  configTime(8 * 3600, 0, "ntp.aliyun.com", "ntp1.aliyun.com",
             "ntp2.aliyun.com");

  Serial.println("正在等待首次时间同步...");

  // 等待首次同步
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 10) {
    Serial.println("等待 NTP 同步...");
    delay(500);
    retry++;
  }

  if (retry >= 10) {
    Serial.println("NTP 同步失败！");
  } else {
    Serial.println("NTP 同步成功！");
    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d %H:%M:%S",
             &timeinfo);
    Serial.print("当前时间：");
    Serial.println(timeStringBuff);
  }

  // 创建时间管理任务
  xTaskCreatePinnedToCore(timeManagerTask, // 任务函数
                          "TimeManager",   // 任务名称
                          4096,            // 增加堆栈大小
                          NULL,            // 任务参数
                          2,               // 提高优先级
                          &tickTaskHandle, // 任务句柄
                          1                // 在核心1上运行
  );
}

void updateTime() {
  // 只设置更新标志，不创建新任务
  updateRequested = true;
}

bool getTimeInfo(struct tm *result) {
  bool success = false;
  if (xSemaphoreTake(timeInfoMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    memcpy(result, &timeinfo, sizeof(struct tm));
    xSemaphoreGive(timeInfoMutex);
    success = true;
  }
  return success;
}

bool isTimeValid() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return false;
  }
  // 检查年份是否合理（2000-2099）
  return (timeinfo.tm_year + 1900) >= 2000 && (timeinfo.tm_year + 1900) <= 2099;
}

} // namespace TimeManager
