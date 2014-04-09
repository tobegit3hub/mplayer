package cn.chendihao.mplayercontroller;

import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.MenuItem.OnMenuItemClickListener;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Toast;

public class MainActivity extends Activity {
	
	private final static String SERVER_IP = "192.168.1.64";
	private final static int SERVER_PORT = 4343;

	private Activity activity;
	private EditText commandText;
	private ImageButton runButton;
	
	private String command;
	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		

		activity = this;

		commandText = (EditText) findViewById(R.id.command_text);
		runButton = (ImageButton) findViewById(R.id.run_button);
		
		
		runButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				
				String commandContent = commandText.getText().toString().trim().toLowerCase();

				if (commandContent.startsWith("play") || commandContent.startsWith("stop") ) {
					command = commandContent;
					sendCommand(command);
				}else{
					Toast.makeText(activity, "Invalid Command", Toast.LENGTH_SHORT).show();
				}

			}

		});
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		
		for (int i = 0; i < menu.size(); i++) {
			menu.getItem(i).setOnMenuItemClickListener(
					new OnMenuItemClickListener() {

						@Override
						public boolean onMenuItemClick(MenuItem item) {
							switch (item.getItemId()) {
							
							case R.id.action_about:

								Toast.makeText(
										activity,
										"Developer:\n\n\n\t吴健松\n\n\t陈迪豪\n\n\t杨伟宏\n\n\t陈远龙\n\n\t文力",
										Toast.LENGTH_LONG).show();

								break;

							case R.id.action_play:

								command = "play ipad2.mp4";
								sendCommand(command);
								
								break;
								
							case R.id.action_stop:
								
								command = "stop";		
								sendCommand(command);
								
								break;
								
							}
							return false;
							
						}
						
					});
		
		}
		
		return true;
	}

	
	@Override
	public void onBackPressed() {
		new AlertDialog.Builder(this)
				.setTitle("Exit Mplayer Controller?")
				.setIcon(R.drawable.ic_launcher)
				.setPositiveButton("Yes",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,
									int which) {

								System.exit(0);
							}
						}).setNegativeButton("Cancel", null).show();
	}
	
	
	void sendCommand(String command){
		try {

			Socket socket = null;
			
			socket = new Socket(SERVER_IP, SERVER_PORT);

			PrintWriter out = new PrintWriter(new BufferedWriter(
					new OutputStreamWriter(socket.getOutputStream())),
					true);

			out.print(command);
			
			Toast.makeText(activity, command, Toast.LENGTH_SHORT).show();

			out.close();
			socket.close();

		} catch (Exception e) {
			e.printStackTrace();
			Toast.makeText(activity, "Socket Exception", Toast.LENGTH_SHORT).show();
		}
	}
	
	
}
