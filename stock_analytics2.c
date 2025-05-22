#include <stdio.h>
#include <math.h>
#include <string.h>

struct Stock {
    char name[50];
    double past_price[5];
    int history_count;

    double avgReturn;
    double volatility;
    double debtRatio;

    char volRiskLevel[10];
    char debtRiskLevel[10];
    char combinedRiskLevel[10];

    double revenuePast;
    double revenueNow;
    double profit;
    double totalDebt;
    double totalAssets;
    double marketPrice;
    double marketCap;

    double peRatio;
    double revenueGrowth;
    double profitMargin;
    double returnOnEquity;
    char scope[50];
};

// utility Functions
double calculate_return(double current, double previous) {
    return (current - previous) / previous;
}

double calculate_mean(double arr[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return sum / n;
}

double calculate_std_dev(double arr[], int n) {
    double mean = calculate_mean(arr, n);
    double sum_squared_diff = 0.0;


    for (int i = 0; i < n; i++) 
    {
        double diff = arr[i] - mean;
        sum_squared_diff += diff * diff;
    }
    return sqrt(sum_squared_diff / (n - 1));
}

int risk_level_to_int(const char *risk) 
{
    if (strcmp(risk, "Low") == 0) return 1;
    if (strcmp(risk, "Medium") == 0) return 2;
    if (strcmp(risk, "High") == 0) return 3;
    return 0;
}

void assess_volatility_risk(double volatility, char *riskLevel) {
    if (volatility < 0.01) strcpy(riskLevel, "Low");
    else if (volatility < 0.02) strcpy(riskLevel, "Medium");

    else strcpy(riskLevel, "High");
}

void assess_debt_risk(double debtRatio, char *riskLevel) {
    if (debtRatio < 0.3) strcpy(riskLevel, "Low");


    else if (debtRatio < 0.6) strcpy(riskLevel, "Medium");
    else strcpy(riskLevel, "High");
}

void assess_combined_risk(const char *volRisk, const char *debtRisk, char *combinedRisk) {
    int vol = risk_level_to_int(volRisk);

    int debt = risk_level_to_int(debtRisk);
    int maxRisk = vol > debt ? vol : debt;


    strcpy(combinedRisk, (maxRisk == 1) ? "Low" : (maxRisk == 2) ? "Medium" : "High");
}

//Metrics Function
void compute_common_metrics(struct Stock *s) {
    double returns[4];
    for (int i = 1; i < 5; i++) {
        returns[i - 1] = calculate_return(s->past_price[i], s->past_price[i - 1]);
    }

    s->avgReturn = calculate_mean(returns, 4);
    s->volatility = calculate_std_dev(returns, 4);
    s->debtRatio = s->totalDebt / s->totalAssets;

    assess_volatility_risk(s->volatility, s->volRiskLevel);
    assess_debt_risk(s->debtRatio, s->debtRiskLevel);
    assess_combined_risk(s->volRiskLevel, s->debtRiskLevel, s->combinedRiskLevel);
}

void short_term_analysis(struct Stock *s) {
    compute_common_metrics(s);

    printf("\n===== Short-Term Analysis for %s =====\n", s->name);
    printf("Average Return: %.2f%%\n", s->avgReturn * 100);

    printf("Volatility    : %.2f%% — Risk: %s\n", s->volatility * 100, s->volRiskLevel);

    printf("Debt Ratio    : %.2f — Risk: %s\n", s->debtRatio, s->debtRiskLevel);

    printf("Combined Risk : %s\n", s->combinedRiskLevel);
}

void long_term_fundamentals(struct Stock *s) 
{
    compute_common_metrics(s);

    printf("Enter revenue 5 years ago (crores): ");
    scanf("%lf", &s->revenuePast);

    printf("Enter current revenue (crores): ");
    scanf("%lf", &s->revenueNow);

    printf("Enter net profit (crores): ");
    scanf("%lf", &s->profit);

    printf("Enter market price per share: ");
    scanf("%lf", &s->marketPrice);

    printf("Enter market cap (crores): ");
    scanf("%lf", &s->marketCap);

    s->peRatio = s->marketCap / s->profit;
    s->revenueGrowth = calculate_return(s->revenueNow, s->revenuePast);
    s->profitMargin = s->profit / s->revenueNow;
    double equity = s->totalAssets - s->totalDebt;
    s->returnOnEquity = (equity > 0) ? s->profit / equity : 0.0;

    int score = 0;
    if (s->peRatio >= 10 && s->peRatio <= 25) 
        score++;
    if (s->revenueGrowth >= 0.30) 
        score++;
    if (s->profitMargin >= 0.10) 
        score++;

    if (s->debtRatio < 0.5) 
        score++;
    if (s->returnOnEquity >= 0.15) 
        score++;

    if (score >= 4) strcpy(s->scope, "Good for long-term hold");
    else if (score >= 2) strcpy(s->scope, "Moderate potential, consider carefully");
    else strcpy(s->scope, "Not recommended for long-term hold");

    printf("\n=== Long-Term Fundamental Analysis: %s ===\n", s->name);
    printf("P/E Ratio          : %.2f\n", s->peRatio);
    printf("Revenue Growth     : %.2f%%\n", s->revenueGrowth * 100);
    printf("Profit Margin      : %.2f%%\n", s->profitMargin * 100);
    printf("Return on Equity   : %.2f%%\n", s->returnOnEquity * 100);
    printf("Debt Ratio         : %.2f\n", s->debtRatio);
    printf("Investment Advice  : %s\n", s->scope);
}

void compare_stocks(struct Stock *s1) 
{
    compute_common_metrics(s1);
    char name2[50];

    double perf2[5];

    printf("Enter second stock name: ");
    scanf("%s", name2);

    printf("Enter 5 past returns for %s:\n", name2);
    for (int i = 0; i < 5; i++) {
        printf("Return %d (as decimal): ", i + 1);
        scanf("%lf", &perf2[i]);
    }

    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;

    for (int i = 0; i < 5; i++) 
    {
        sumX += s1->past_price[i];
        sumY += perf2[i];
        sumXY += s1->past_price[i] * perf2[i];
        sumX2 += s1->past_price[i] * s1->past_price[i];
        sumY2 += perf2[i] * perf2[i];
    }

    // Pearson correlation formula
    double den = sqrt((5 * sumX2 - sumX * sumX) * (5 * sumY2 - sumY * sumY));
    double r = (den == 0) ? 0 : 5 * sumXY - sumX * sumY / sqrt((5 * sumX2 - sumX * sumX) * (5 * sumY2 - sumY * sumY)); //if denominator is 0, coefficient is 0

    printf("Stock Comparison\n");
    printf("Correlation Coefficient: %.4f\n", r);

    if (r > 0.7)
        printf("Positive correlation, stocks tend to move together\n");

    else if (r < -0.7)
        printf("Negative correlation, these stocks tend to move in opposite directions\n");

    else if (fabs(r) < 0.3)
        printf("weak/ no correlation\n"); //unrelated stock 

    else
        printf("moderate correlation\n");// some relationship exists

    printf("\nRisk Profiles:\n");
    printf("%-15s %-25s %-15s\n", "Stock", "Volatility", "Debt Risk");// left aligned strings 15, 25 and 15 character widths
    printf("%-15s %.2f%% (%s)               %-15s\n", s1->name, s1->volatility * 100, s1->volRiskLevel, s1->debtRiskLevel); // .2f%% volatility as a % with 2 decimal places
    // %s is the volatility risk level
    //spaces for alginment 
    //s1->debtRisklevel debt risk level
    printf("%-15s %-25s %-15s\n", name2, "N/A", "N/A"); // second stock placeholder
}

void investment_advice(struct Stock s) {
    compute_common_metrics(&s);

    float latest_change = calculate_return(s.past_price[0], s.past_price[1]);
    float sum = 0.0;


    for (int i = 2; i < 5; i++) {
        sum += calculate_return(s.past_price[i], s.past_price[i - 1]);
    }

    float avg_change = sum / 3.0;

    printf("\nANALYSIS RESULT for %s:\n", s.name);
    printf("Latest Change: %.2f%%\n", latest_change * 100);
    printf("Average of Previous Changes: %.2f%%\n", avg_change * 100);

    if (latest_change > avg_change * 1.5) {
        printf("Recommendation: BUY\n");
        printf("Reason: The latest price increase is significantly above the average momentum, indicating strong upward trend.\n");
    }

    else if (latest_change < avg_change * 0.5){
        printf("Recommendation: SELL\n");
        printf("Reason: The latest drop is much worse than the average performance, indicating a loss of momentum.\n");
    } 
    else 
    {
        printf("Recommendation: HOLD\n");
        printf("Reason: The latest change is within a normal range compared to historical changes. No strong momentum detected.\n");
    }

    printf("\nRisk Profile:\n");
    printf("- Volatility Risk: %s\n", s.volRiskLevel);
    printf("- Debt Risk: %s\n", s.debtRiskLevel);
    printf("- Combined Risk: %s\n", s.combinedRiskLevel);
}

int main() {
    struct Stock stock1;
    stock1.history_count = 5;

    printf("Enter stock name: ");
    scanf("%s", stock1.name);

    printf("Enter 5 daily prices for %s:\n", stock1.name);
    for (int i = 0; i < 5; i++) {
        printf("Price on Day %d: ", i + 1);
        scanf("%lf", &stock1.past_price[i]);
    }

    printf("Enter total debt (in crores): ");
    scanf("%lf", &stock1.totalDebt);
    printf("Enter total assets (in crores): ");
    scanf("%lf", &stock1.totalAssets);

    int choice;
    do {
        printf("\n====== STOCK ANALYSIS MENU ======\n");
        printf("1. Short-Term Risk Analysis\n");
        printf("2. Long-Term Fundamental Evaluation\n");
        printf("3. Compare with Another Stock (Correlation)\n");
        printf("4. Investment Advice\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: short_term_analysis(&stock1); break;
            case 2: long_term_fundamentals(&stock1); break;
            case 3: compare_stocks(&stock1); break;
            case 4: investment_advice(stock1); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid option. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
