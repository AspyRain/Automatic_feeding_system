package cqjtu.afs_mobile.entity;

public class FeedingDevice {
    private long id;
    private String name;

    private boolean switchChecked=false;

    public boolean isSwitchChecked(){
        return switchChecked;
    }
    public void setSwitchChecked(boolean switchChecked){
        this.switchChecked=switchChecked;
    }
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
