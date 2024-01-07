from moviepy.editor import VideoFileClip

def convert_mov_to_gif(input_file, output_file, fps=5, scale_factor=0.5):
    # 读取MOV文件，指定audio=False来忽略音频
    video_clip = VideoFileClip(input_file, audio=False)
    
    # 设置帧率（Frames Per Second）
    video_clip = video_clip.set_fps(fps)
    
    # 调整分辨率
    video_clip = video_clip.resize(scale_factor)
    
    # 生成GIF文件
    video_clip.write_gif(output_file)

if __name__ == "__main__":
    # 输入和输出文件路径
    input_files = ["./IMG_0264.MOV"]  # 你的MOV文件列表
    output_files = ["output1.gif"]  # 对应的输出GIF文件名
    
# 设置帧率和缩放因子
    target_fps = 5  # 你可以适当调整这个值
    scale_factor = 0.15  # 设置缩放因子，可以根据需要调整
    
    # 批量转换
    for i in range(len(input_files)):
        convert_mov_to_gif(input_files[i], output_files[i], fps=target_fps, scale_factor=scale_factor)