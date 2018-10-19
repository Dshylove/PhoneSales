create Procedure sp_SelectUser
as
select username from Userlogin

create Procedure sp_SelectPassword
@username nvarchar(20)
as
select password from Userlogin where username=@username

create Procedure sp_UpdateStatus
@deliverytime datetime,
@orderID varchar(15)
as
update Orders set status='�ѷ���',deliverytime=@deliverytime where orderID=@orderID

create Procedure sp_InsertOrders
@orderID varchar(15),
@tradecode int,
@price float,
@amount int,
@totalprice float,
@name nvarchar(10),
@Tel varchar(12),
@address nvarchar(50),
@date datetime
as
insert into Orders(orderID,tradecode,price,amount,totalprice,status,name,Tel,address,date)
 values(@orderID,@tradecode,@price,@amount,@totalprice,'δ����',@name,@Tel,@address,@date)



--�ж������Ƿ���ȷ--
create Procedure sp_Password_IsRight
@username nvarchar(20),
@password nvarchar(20),
@IsRight bit output
as
declare @temp_pwd nvarchar(20);
select @temp_pwd=password from Userlogin where username=@username;
if(@temp_pwd=@password)
	set @IsRight=1;
else
	set @IsRight=0;


declare @m bit
exec sp_Password_IsRight 'one','123',@m output
select @m



--�ж��û��������Ƿ���ȷ--
alter Procedure sp_Userlogin_IsRight
@username nvarchar(20),
@password nvarchar(20),
@IsRight int output
as
declare @temp_pwd nvarchar(20);
select @temp_pwd=password from Userlogin where username=@username;
if(@@ROWCOUNT=0)
	set @IsRight=0;		--�û��������ڲ�������0
else
begin
	if(@temp_pwd=@password)
		set @IsRight=2;	--������ȷ��������2
	else
		set @IsRight=1;	--��������������1
end


declare @m int
exec sp_Userlogin_IsRight 'admin','admin',@m output
select @m