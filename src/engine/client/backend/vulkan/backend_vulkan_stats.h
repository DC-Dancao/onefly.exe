 
#ifndef ENGINE_CLIENT_BACKEND_VULKAN_BACKEND_VULKAN_STATS_H
#define ENGINE_CLIENT_BACKEND_VULKAN_BACKEND_VULKAN_STATS_H

#include <atomic>
#include <cstdint>




struct SVulkanLatencyStats
{
	std::atomic<bool> m_Supported{false};
	std::atomic<bool> m_ModeActive{false};
	std::atomic<bool> m_BoostActive{false};
	std::atomic<bool> m_PresentWaitSupported{false};
	std::atomic<bool> m_PresentWaitActive{false};
	std::atomic<uint32_t> m_SleepTimeoutCount{0};
	std::atomic<uint64_t> m_SleepAvgUs{0};
	std::atomic<uint64_t> m_SleepMaxUs{0};
	std::atomic<uint64_t> m_PresentWaitAvgUs{0};
	std::atomic<uint64_t> m_PresentWaitMaxUs{0};
};

inline SVulkanLatencyStats g_VulkanLatencyStats;

#endif
