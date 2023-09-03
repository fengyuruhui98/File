package com.zdd.service;
import com.zdd.dao.UserDao;

public class UserService {
    public static void testService(){
        System.out.println("UserService test...");
        //调用userdao的方法
        UserDao.testDao();
    }
}
