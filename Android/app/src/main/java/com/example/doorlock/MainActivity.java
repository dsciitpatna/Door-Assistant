package com.example.doorlock;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.FirebaseApp;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.messaging.FirebaseMessaging;

import static android.content.ContentValues.TAG;


public class MainActivity extends AppCompatActivity {
    FirebaseDatabase database;
    DatabaseReference sensor_status;
    DatabaseReference door_open;

    ToggleButton sensor_status_button;
    TextView door_status_text;

    Context context;
    /*

FirebaseDatabase database = FirebaseDatabase.getInstance();
DatabaseReference myRef = database.getReference("message");

myRef.setValue("Hello, World!");

myRef.addValueEventListener(new ValueEventListener() {
    @Override
    public void onDataChange(DataSnapshot dataSnapshot) {
        // This method is called once with the initial value and again
        // whenever data at this location is updated.
        String value = dataSnapshot.getValue(String.class);
        Log.d(TAG, "Value is: " + value);
    }

    @Override
    public void onCancelled(DatabaseError error) {
        // Failed to read value
        Log.w(TAG, "Failed to read value.", error.toException());
    }
});
 */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        sensor_status_button = findViewById(R.id.sensor_status);
        door_status_text = findViewById(R.id.door_status);

        FirebaseApp.initializeApp(this);
        database = FirebaseDatabase.getInstance();
        sensor_status = database.getReference("sensor_status");
        door_open = database.getReference("door_open");

        context = this;

        FirebaseMessaging.getInstance().subscribeToTopic("door_status")
                .addOnCompleteListener(new OnCompleteListener<Void>() {
                    @Override
                    public void onComplete(@NonNull Task<Void> task) {
                        String msg = "Subscribed to door_status";
                        if (!task.isSuccessful()) {
                            msg = "Couldn't subscribe";
                        }
                        Log.d("mytag", msg);
                    }
                });

        door_open.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                Boolean value = dataSnapshot.getValue(Boolean.class);
                if (value) {
                    door_status_text.setText("DOOR: OPEN");
                } else {
                    door_status_text.setText("DOOR: CLOSED");
                }
                Log.d(TAG, "Value is: " + value);
            }

            @Override
            public void onCancelled(DatabaseError error) {
                // Failed to read value
                Log.w(TAG, "Failed to read value.", error.toException());
            }
        });

        sensor_status_button.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton b, boolean checked) {
                if (checked) {
                    sensor_status.setValue(true);
                    Log.w(TAG, "checked");

                } else {
                    sensor_status.setValue(false);
                    Log.w(TAG, "unchecked");
                }
            }
        });

    }
}
