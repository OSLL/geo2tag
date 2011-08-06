package ru.spb.osll.web.server;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import ru.spb.osll.web.client.GreetingService;
import ru.spb.osll.web.server.db.common.DBUtil;
import ru.spb.osll.web.shared.FieldVerifier;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@SuppressWarnings("serial")
public class GreetingServiceImpl extends RemoteServiceServlet implements
    GreetingService {

  public String greetServer(String input) throws IllegalArgumentException {
    if (!FieldVerifier.isValidName(input)) {
      throw new IllegalArgumentException(
          "Name must be at least 4 characters long");
    }
    try{
		Connection c = DBUtil.getConnection();
		Statement Ex1Stmt = c.createStatement();
		ResultSet result = Ex1Stmt.executeQuery("select login from users;");
	    while (result.next()) {
			input += "\n" + result.getString("login");
		}
	} catch (SQLException e) {
		e.printStackTrace();
	}
    return "Hello, " + input;
  }

}
