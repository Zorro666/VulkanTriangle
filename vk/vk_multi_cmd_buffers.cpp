/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2022 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include "vk_test.h"

RD_TEST(VK_Multi_Cmd_Buffers, VulkanGraphicsTest)
{
  static constexpr const char *Description =
    "Draws multiple triangles using multiple command buffers";

  int main()
  {
    // initialise, create window, create context, etc
    if(!Init())
      return 3;

    VkPipelineLayout layout = createPipelineLayout(vkh::PipelineLayoutCreateInfo());

    vkh::GraphicsPipelineCreateInfo pipeCreateInfo;

    pipeCreateInfo.layout = layout;
    pipeCreateInfo.renderPass = mainWindow->rp;

    pipeCreateInfo.vertexInputState.vertexBindingDescriptions = {vkh::vertexBind(0, DefaultA2V)};
    pipeCreateInfo.vertexInputState.vertexAttributeDescriptions = {
        vkh::vertexAttr(0, 0, DefaultA2V, pos), vkh::vertexAttr(1, 0, DefaultA2V, col),
        vkh::vertexAttr(2, 0, DefaultA2V, uv),
    };

    pipeCreateInfo.stages = {
        CompileShaderModule(VKDefaultVertex, ShaderLang::glsl, ShaderStage::vert, "main"),
        CompileShaderModule(VKDefaultPixel, ShaderLang::glsl, ShaderStage::frag, "main"),
    };

    VkPipeline pipe = createGraphicsPipeline(pipeCreateInfo);

    AllocatedBuffer vb(
        this, vkh::BufferCreateInfo(sizeof(DefaultTri), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                                                            VK_BUFFER_USAGE_TRANSFER_DST_BIT),
        VmaAllocationCreateInfo({0, VMA_MEMORY_USAGE_CPU_TO_GPU}));
    vb.upload(DefaultTri);

    AllocatedBuffer vb2(
        this, vkh::BufferCreateInfo(sizeof(DefaultTri), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                                                            VK_BUFFER_USAGE_TRANSFER_DST_BIT),
        VmaAllocationCreateInfo({0, VMA_MEMORY_USAGE_CPU_TO_GPU}));
    vb2.upload(DefaultTri);
    DefaultA2V *triangle2 = (DefaultA2V *)vb2.map();

    while(Running())
    {
      std::vector<VkCommandBuffer> cmdBuffers;

      VkCommandBuffer cmdClear =  GetCommandBuffer();
      vkBeginCommandBuffer(cmdClear, vkh::CommandBufferBeginInfo());
      VkImage swapimg =
          StartUsingBackbuffer(cmdClear, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_GENERAL);

      vkCmdClearColorImage(cmdClear, swapimg, VK_IMAGE_LAYOUT_GENERAL,
                           vkh::ClearColorValue(0.9f, 0.2f, 0.2f, 1.0f), 1,
                           vkh::ImageSubresourceRange());
      vkEndCommandBuffer(cmdClear);

      VkCommandBuffer cmd1 = GetCommandBuffer();
      vkBeginCommandBuffer(cmd1, vkh::CommandBufferBeginInfo());
      vkCmdBeginRenderPass(
          cmd1, vkh::RenderPassBeginInfo(mainWindow->rp, mainWindow->GetFB(), mainWindow->scissor),
          VK_SUBPASS_CONTENTS_INLINE);
      vkCmdBindPipeline(cmd1, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe);
      vkCmdSetViewport(cmd1, 0, 1, &mainWindow->viewport);
      vkCmdSetScissor(cmd1, 0, 1, &mainWindow->scissor);
      vkh::cmdBindVertexBuffers(cmd1, 0, {vb.buffer}, {0});
      vkCmdDraw(cmd1, 3, 1, 0, 0);
      vkCmdEndRenderPass(cmd1);
      vkEndCommandBuffer(cmd1);

      VkCommandBuffer cmd2 = GetCommandBuffer();
      vkBeginCommandBuffer(cmd2, vkh::CommandBufferBeginInfo());
      vkCmdBeginRenderPass(
          cmd2, vkh::RenderPassBeginInfo(mainWindow->rp, mainWindow->GetFB(), mainWindow->scissor),
          VK_SUBPASS_CONTENTS_INLINE);
      vkCmdBindPipeline(cmd2, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe);
      vkCmdSetViewport(cmd2, 0, 1, &mainWindow->viewport);
      vkCmdSetScissor(cmd2, 0, 1, &mainWindow->scissor);
      vkh::cmdBindVertexBuffers(cmd2, 0, {vb2.buffer}, {0});
      for(int i = 0; i < 3; ++i)
      {
        triangle2[i].pos.x = DefaultTri[i].pos.x + 0.4f;
        triangle2[i].pos.y = DefaultTri[i].pos.y - 0.3f;
        triangle2[i].col.x = 1.0f;
        triangle2[i].col.y = 0.0f;
        triangle2[i].col.z = 0.0f;
      }
      vkCmdDraw(cmd2, 3, 1, 0, 0);
      vkCmdEndRenderPass(cmd2);
      vkEndCommandBuffer(cmd2);

      cmdBuffers.push_back(cmdClear);
      cmdBuffers.push_back(cmd2);
      cmdBuffers.push_back(cmd1);
      Submit(0, 1, cmdBuffers);
      cmdBuffers.clear();

      Sync();
      VkCommandBuffer cmd3 = GetCommandBuffer();
      vkBeginCommandBuffer(cmd3, vkh::CommandBufferBeginInfo());
      vkCmdBeginRenderPass(
          cmd3, vkh::RenderPassBeginInfo(mainWindow->rp, mainWindow->GetFB(), mainWindow->scissor),
          VK_SUBPASS_CONTENTS_INLINE);
      vkCmdBindPipeline(cmd3, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe);
      vkCmdSetViewport(cmd3, 0, 1, &mainWindow->viewport);
      vkCmdSetScissor(cmd3, 0, 1, &mainWindow->scissor);
      vkh::cmdBindVertexBuffers(cmd3, 0, {vb2.buffer}, {0});
      for(int i = 0; i < 3; ++i)
      {
        triangle2[i].pos.x = DefaultTri[i].pos.x - 0.4f;
        triangle2[i].pos.y = DefaultTri[i].pos.y - 0.3f;
        triangle2[i].col.x = 0.0f;
        triangle2[i].col.y = 0.0f;
        triangle2[i].col.z = 1.0f;
      }

      vkCmdDraw(cmd3, 3, 1, 0, 0);
      vkCmdEndRenderPass(cmd3);
      vkEndCommandBuffer(cmd3);
      Submit(1, 1, {cmd3});

      Present();
    }
    vb2.unmap();

    return 0;
  }
};

REGISTER_TEST();
