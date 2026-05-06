public class TownCrier {
    int announcedMessageCount;
    String message;

    TownCrier(){
        this.announcedMessageCount = 0;
    }

    void setMessage(String message){
        this.message = message;
        this.announcedMessageCount = 0;
    }

    String announce(){
        this.announcedMessageCount++;
        return this.message;
    }

    int getNumberOfLastMessageAnnounced(){
        return this.announcedMessageCount;
    }
}
