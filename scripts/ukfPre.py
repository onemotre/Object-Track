from filterpy.kalman import UnscentedKalmanFilter, MerweScaledSigmaPoints
import numpy as np

# 创建无迹卡尔曼滤波器对象
dim_x = 4  # 状态维度（位置x, 速度x, 位置y, 速度y）
dim_z = 2  # 观测维度（位置x, 位置y）
ukf = UnscentedKalmanFilter(dim_x=dim_x, dim_z=dim_z)

# 设置状态转移函数（平面运动模型）
def state_transition_function(x, dt):
    x_, vx, y_, vy = x
    x_new = x_ + vx * dt
    y_new = y_ + vy * dt
    return np.array([x_new, vx, y_new, vy])

ukf.fx = state_transition_function

# 设置观测函数（位置观测）
def observation_function(x):
    return np.array([x[0], x[2]])

ukf.hx = observation_function

# 设置无迹点数
points = MerweScaledSigmaPoints(n=dim_x, alpha=0.1, beta=2.0, kappa=1.0)
ukf.points = points

# 模拟观测数据
observations = np.array([
    [1.0, 1.0],
    [2.0, 2.0],
    [3.0, 3.0],
])

# 初始化状态估计和协方差矩阵
initial_x = observations[0][0]
initial_y = observations[0][1]
ukf.x = np.array([initial_x, 0.0, initial_y, 0.0])
ukf.P *= np.eye(dim_x) * 1000

# 设置观测噪声协方差矩阵
ukf.R *= np.eye(dim_z) * 5

# 逐步进行预测和更新
predicted_positions = []
for z in observations:
    ukf.predict()
    ukf.update(z)
    predicted_positions.append(ukf.x[:2])  # 提取位置坐标

print("观测数据:", observations)
print("预测的位置:", predicted_positions)
