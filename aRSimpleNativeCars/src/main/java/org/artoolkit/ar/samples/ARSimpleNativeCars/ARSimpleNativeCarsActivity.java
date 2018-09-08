/*
 *  ARSimpleNativeCarsActivity.java
 *  ARToolKit5
 *
 *  Disclaimer: IMPORTANT:  This Daqri software is supplied to you by Daqri
 *  LLC ("Daqri") in consideration of your agreement to the following
 *  terms, and your use, installation, modification or redistribution of
 *  this Daqri software constitutes acceptance of these terms.  If you do
 *  not agree with these terms, please do not use, install, modify or
 *  redistribute this Daqri software.
 *
 *  In consideration of your agreement to abide by the following terms, and
 *  subject to these terms, Daqri grants you a personal, non-exclusive
 *  license, under Daqri's copyrights in this original Daqri software (the
 *  "Daqri Software"), to use, reproduce, modify and redistribute the Daqri
 *  Software, with or without modifications, in source and/or binary forms;
 *  provided that if you redistribute the Daqri Software in its entirety and
 *  without modifications, you must retain this notice and the following
 *  text and disclaimers in all such redistributions of the Daqri Software.
 *  Neither the name, trademarks, service marks or logos of Daqri LLC may
 *  be used to endorse or promote products derived from the Daqri Software
 *  without specific prior written permission from Daqri.  Except as
 *  expressly stated in this notice, no other rights or licenses, express or
 *  implied, are granted by Daqri herein, including but not limited to any
 *  patent rights that may be infringed by your derivative works or by other
 *  works in which the Daqri Software may be incorporated.
 *
 *  The Daqri Software is provided by Daqri on an "AS IS" basis.  DAQRI
 *  MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 *  THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE, REGARDING THE DAQRI SOFTWARE OR ITS USE AND
 *  OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 *
 *  IN NO EVENT SHALL DAQRI BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 *  MODIFICATION AND/OR DISTRIBUTION OF THE DAQRI SOFTWARE, HOWEVER CAUSED
 *  AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 *  STRICT LIABILITY OR OTHERWISE, EVEN IF DAQRI HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  Copyright 2015 Daqri, LLC.
 *  Copyright 2011-2015 ARToolworks, Inc.
 *
 *  Author(s): Julian Looser, Philip Lamb
 *
 */

package org.artoolkit.ar.samples.ARSimpleNativeCars;

import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Vibrator;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.FrameLayout;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;

import org.artoolkit.ar.base.ARActivity;
import org.artoolkit.ar.base.rendering.ARRenderer;

import android.view.animation.Animation;
import android.view.animation.AnimationUtils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class ARSimpleNativeCarsActivity extends ARActivity {

    private SimpleNativeRenderer simpleNativeRenderer = new SimpleNativeRenderer();


    private FrameLayout mainLayout;
    public TextView textView;
    private View view;

    private int countdown = 90;

    public final int ITEM_NUM = 3;
    private boolean[] find_item=new boolean[ITEM_NUM];
    private boolean[] use_item=new boolean[ITEM_NUM];
    private static final String TAG = "tag";

    static public int solvenum = 0;
    public int solvecount = 0;


    Animation animbounce;

    static public SimpleAdapter saImageItems;
    static public ArrayList<Map<String,Object>> Item = new ArrayList<Map<String, Object>>();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        /****************** CountDownTimer *******************************************/
        textView = (TextView)this.findViewById(R.id.textView);
        textView.setText(Integer.toString(countdown));

        animbounce = AnimationUtils.loadAnimation(getApplicationContext(),
                R.anim.bounce);


        new CountDownTimer(countdown*1000,1000){

            @Override
            public void onFinish() {
                textView.setPadding(20,75,0,0);
                textView.setText("End");

                if(solvecount == 3){
                    textView.setText("Win");
                    textView.startAnimation(animbounce);
                }
                else{
                    textView.setText("bye");
                    textView.startAnimation(animbounce);
                }
                //textView.setText("0:00");
            }

            @Override
            public void onTick(long millisUntilFinished) {
                textView.setText(String.valueOf(millisUntilFinished/1000/60)+":"+String.valueOf((millisUntilFinished/1000)%60));
            }

        }.start();

        /****************** About click setup *******************************************/
        mainLayout = (FrameLayout)this.findViewById(R.id.mainLayout);

        init();

        for (int i = 0; i < ITEM_NUM; i++) {
            HashMap<String, Object> temp = new HashMap<String, Object>();
            temp.put("img", R.drawable.icon0);
            temp.put("name", "");
            temp.put("num", "");
            Item.add(temp);
        }

        saImageItems = new SimpleAdapter(getApplicationContext(),
                Item,
                R.layout.list,
                new String[] { "img", "name","num"},
                new int[] { R.id.img, R.id.name,R.id.num});

        view = findViewById(R.id.listview);
        view.getBackground().setAlpha(0);
        ((ListView) view).setAdapter(saImageItems);


        /****************** MainLayout click detect *******************************************/
        // When the screen is tapped, inform the renderer and vibrate the phone
        mainLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                Vibrator vib = (Vibrator) getSystemService(VIBRATOR_SERVICE);
                vib.vibrate(20);

                int getID = simpleNativeRenderer.click();

                //////////////key items//////////////////////////////
                if (getID != -1 && getID < ITEM_NUM) {
                    HashMap<String, Object> temp = new HashMap<String, Object>();
                    switch (getID) {
                        case 0:
                            temp.put("img", R.drawable.sword);
                            temp.put("name", "item no.0");
                            break;
                        case 1:
                            temp.put("img", R.drawable.key2);
                            temp.put("name", "item no.1");
                            break;
                        default:
                            temp.put("img", R.drawable.gta);
                            temp.put("name", "item");
                            break;
                    }
                    temp.put("num", "");

                    if (!find_item[getID]) {
                        Item.remove(getID);
                        Item.add(getID, temp);
                        find_item[getID] = true;
                    }
                    saImageItems.notifyDataSetChanged();

                }





            }

        });


        /****************** ListView click detect *******************************************/
        ((ListView) view).setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View v, int position, long id) {

                Vibrator vib = (Vibrator) getSystemService(VIBRATOR_SERVICE);
                vib.vibrate(20);

                int itemID = position;
                if (find_item[itemID] && !use_item[itemID]) {

                    //Toast.makeText(getApplicationContext(), "use item no." + position , Toast.LENGTH_SHORT).show();

                    ///// check whether key match puzzle////
                    int puzzleID = simpleNativeRenderer.click();

                    Log.e("itemID, puzzleID", String.valueOf(itemID) +", "+ String.valueOf(puzzleID));

                    if(puzzleID >= ITEM_NUM){ //is puzzle, not others

                        if(puzzleID == 3 && itemID == 0) { // Locker and key
                            Log.e("Success Match","Enemy and Sword");
                            solvenum = 3; solvecount++;

                            Toast.makeText(getApplicationContext(), "use item no." + position , Toast.LENGTH_SHORT).show();
                            Item.remove(itemID);
                            HashMap<String, Object> temp = new HashMap<String, Object>();
                            temp.clear();
                            temp.put("img", R.drawable.icon0);
                            temp.put("name", "");
                            temp.put("num", "");
                            Item.add(itemID, temp);
                            use_item[itemID] = true;
                        }
                        if(puzzleID == 4 && itemID == 1) { // Locker and key
                            Log.e("Success Match","Locker and Key");
                            solvenum = 4; solvecount++;

                            Toast.makeText(getApplicationContext(), "use item no." + position , Toast.LENGTH_SHORT).show();
                            Item.remove(itemID);
                            HashMap<String, Object> temp = new HashMap<String, Object>();
                            temp.clear();
                            temp.put("img", R.drawable.icon0);
                            temp.put("name", "");
                            temp.put("num", "");
                            Item.add(itemID, temp);
                            use_item[itemID] = true;
                        }
                        if(puzzleID == 5 && itemID == 2) { // Locker and key
                            Log.e("Success Match","Pokeman and Hostage");
                            solvenum = 5; solvecount++;

                            Toast.makeText(getApplicationContext(), "use item no." + position , Toast.LENGTH_SHORT).show();
                            Item.remove(itemID);
                            HashMap<String, Object> temp = new HashMap<String, Object>();
                            temp.clear();
                            temp.put("img", R.drawable.icon0);
                            temp.put("name", "");
                            temp.put("num", "");
                            Item.add(itemID, temp);
                            use_item[itemID] = true;
                        }
                    }

                }
                else
                    Toast.makeText(getApplicationContext(), "empty " + position, Toast.LENGTH_SHORT).show();
                saImageItems.notifyDataSetChanged();


                if(solvecount==3){
                    textView.setText("Win");
                    textView.startAnimation(animbounce);
                    finish();
                }
            }
        });

    }

    public void onStop() {
        SimpleNativeRenderer.demoShutdown();

        super.onStop();
    }

    @Override
    protected ARRenderer supplyRenderer() {
        return simpleNativeRenderer;
    }

    @Override
    protected FrameLayout supplyFrameLayout() {
        return (FrameLayout) this.findViewById(R.id.mainLayout);

    }




    public void init(){
        for(int i=0;i<ITEM_NUM;i++){
            find_item[i]=false;
            use_item[i]=false;
        }

    }
}