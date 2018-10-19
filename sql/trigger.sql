--查看所有触发器
select * from
sysobjects where
xtype='TR'

exec sp_helptext 'T_Orders_update'

create trigger T_Orders_insert on Orders for insert as
begin
declare @name nvarchar(10);
declare @Tel varchar(12);
declare @address nvarchar(25);

select @name=name,@Tel=Tel,@address=address from inserted;
insert into Customer(name,Tel,address) values(@name,@Tel,@address); 
end


create trigger T_Orders_update on Orders for update as
begin
declare @name nvarchar(10);
declare @Tel varchar(12);
declare @address nvarchar(25);

select @name=name,@Tel=Tel,@address=address from inserted;
update Customer set name=@name,Tel=@Tel,address=@address where ID=(select ID from inserted);
end