/* Loads the vendor Vulkan driver through the Android loader, lists the GPU and creates a device. */
#include <stdio.h>
#include <vulkan/vulkan.h>

int main(void) {
    VkApplicationInfo app = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, .apiVersion = VK_API_VERSION_1_0};
    VkInstanceCreateInfo ici = {.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, .pApplicationInfo = &app};
    VkInstance inst;
    VkResult r = vkCreateInstance(&ici, NULL, &inst);
    printf("vkCreateInstance: %d\n", r);
    if (r) return 1;
    uint32_t n = 0;
    r = vkEnumeratePhysicalDevices(inst, &n, NULL);
    printf("vkEnumeratePhysicalDevices: %d, %u device(s)\n", r, n);
    if (!n) return 2;
    VkPhysicalDevice pd[4];
    if (n > 4) n = 4;
    vkEnumeratePhysicalDevices(inst, &n, pd);
    VkPhysicalDeviceProperties p;
    vkGetPhysicalDeviceProperties(pd[0], &p);
    printf("device: %s, api %u.%u.%u, driver 0x%x, vendor 0x%x\n", p.deviceName, VK_VERSION_MAJOR(p.apiVersion),
           VK_VERSION_MINOR(p.apiVersion), VK_VERSION_PATCH(p.apiVersion), p.driverVersion, p.vendorID);
    float prio = 1.0f;
    VkDeviceQueueCreateInfo q = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, .queueFamilyIndex = 0,
                                 .queueCount = 1, .pQueuePriorities = &prio};
    VkDeviceCreateInfo dci = {.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, .queueCreateInfoCount = 1,
                              .pQueueCreateInfos = &q};
    VkDevice dev;
    r = vkCreateDevice(pd[0], &dci, NULL, &dev);
    printf("vkCreateDevice: %d\n", r);
    if (!r) vkDestroyDevice(dev, NULL);
    vkDestroyInstance(inst, NULL);
    return 0;
}
