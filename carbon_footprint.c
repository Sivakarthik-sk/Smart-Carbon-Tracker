#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "carbon_log.csv"

typedef struct {
    char date[20];
    float electricity;      // kWh
    float fuel;             // km
    float publicTransport;  // km
    int dietType;           // 1=Veg,2=Mixed,3=NonVeg
    float totalCO2;         // kg CO2/day
} Entry;

// ---------- Utility ----------
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

float calculateCO2(Entry d) {
    float e = d.electricity * 0.92f;       // Electricity
    float f = d.fuel * 0.12f;              // Fuel
    float p = d.publicTransport * 0.05f;   // Public transport
    float diet = (d.dietType==1)?2.0f:(d.dietType==2)?3.0f:4.0f;
    return e+f+p+diet;
}

void saveEntry(Entry e) {
    FILE *fp = fopen(DATA_FILE,"a");
    if(!fp){printf("Error saving data!\n"); return;}
    fprintf(fp,"%s,%.2f,%.2f,%.2f,%d,%.2f\n",
            e.date,e.electricity,e.fuel,e.publicTransport,e.dietType,e.totalCO2);
    fclose(fp);
}

void showTips(Entry e) {
    printf("\n🌿 Eco Suggestions:\n");
    if(e.electricity>10) printf("- Switch to LED bulbs & solar panels.\n");
    if(e.fuel>20) printf("- Try carpooling or EVs.\n");
    if(e.publicTransport<5) printf("- Use public transport more often.\n");
    if(e.dietType==3) printf("- Try meat-free days once a week.\n");
    printf("- Turn off appliances when not in use.\n");
}

// ---------- Features ----------
void addNewEntry() {
    Entry e;
    clearScreen();
    printf("Date (DD/MM/YYYY): ");
    scanf("%s", e.date);
    printf("Electricity usage (kWh): ");
    scanf("%f",&e.electricity);
    printf("Private transport distance (km): ");
    scanf("%f",&e.fuel);
    printf("Public transport distance (km): ");
    scanf("%f",&e.publicTransport);
    printf("Diet (1=Veg, 2=Mixed, 3=Non-Veg): ");
    scanf("%d",&e.dietType);

    e.totalCO2 = calculateCO2(e);
    printf("\nYour total CO2 emission for %s: %.2f kg CO2\n",e.date,e.totalCO2);
    printf("Equivalent trees needed yearly: %.2f\n",(e.totalCO2*365)/22.0);
    saveEntry(e);
    showTips(e);
    printf("\n✅ Data saved to %s\n",DATA_FILE);
}

void generateReport() {
    FILE *fp=fopen(DATA_FILE,"r");
    if(!fp){printf("No data file found!\n");return;}
    Entry e;
    float sum=0,max=0,min=9999;
    int count=0;
    char line[200];
    printf("\n📊 Carbon Emission Report\n---------------------------------------\n");
    printf("%-12s %-12s\n","Date","Total CO2(kg)");
    while(fgets(line,sizeof(line),fp)){
        sscanf(line,"%[^,],%f,%f,%f,%d,%f",
               e.date,&e.electricity,&e.fuel,&e.publicTransport,&e.dietType,&e.totalCO2);
        printf("%-12s %-12.2f\n",e.date,e.totalCO2);
        sum+=e.totalCO2;
        if(e.totalCO2>max) max=e.totalCO2;
        if(e.totalCO2<min) min=e.totalCO2;
        count++;
    }
    fclose(fp);
    if(count>0){
        printf("\nAverage: %.2f kg\n",sum/count);
        printf("Highest: %.2f kg | Lowest: %.2f kg\n",max,min);
        printf("Annual offset needed: %.2f trees\n",(sum/count*365)/22.0);
    } else printf("No entries recorded yet.\n");
}

void clearData(){
    printf("Are you sure you want to erase all records? (y/n): ");
    char c; scanf(" %c",&c);
    if(c=='y'||c=='Y'){
        remove(DATA_FILE);
        printf("All records cleared.\n");
    } else printf("Cancelled.\n");
}

// ---------- Main ----------
int main(){
    int choice;
    do{
        printf("\n==============================\n");
        printf("  🌍 EcoCalc Pro - Smart Carbon Tracker\n");
        printf("==============================\n");
        printf("1. Add new daily entry\n");
        printf("2. View emission report\n");
        printf("3. Clear all data\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d",&choice);
        switch(choice){
            case 1: addNewEntry(); break;
            case 2: generateReport(); break;
            case 3: clearData(); break;
            case 0: printf("Goodbye! Stay eco-friendly 🌱\n"); break;
            default: printf("Invalid choice!\n");
        }
    }while(choice!=0);
    return 0;
}
