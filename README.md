# 手机零售系统（Qt桌面应用）

技术栈：Qt + SQL Server，零售端+管理端二合一。数据库系统大作业。

使用了SQL Server的存储过程、触发器。

### 存储过程
- 登录验证，用户名和密码的判断逻辑以及SQL语句写在存储过程中 (sp_Userlogin_IsRight)
- 商家下单时，订单信息插入到订单表的SQL语句写在存储过程中 (sp_InsertOrders)
- 商家发货时，更新订单状态的SQL语句写在存储过程中 (sp_UpdateStatus)

### 触发器
- 当订单表新增记录时，自动将对应客户信息新增到客户表中 (T_Orders_insert)
- 当订单表更新记录时，自动将新的客户信息更新到客户表中 (T_Orders_update)

## 界面

![界面](https://github.com/Dshylove/images/raw/master/PhoneSales/interface.png)