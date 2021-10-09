
//

module sim_common_bypass_buffer (
    input   wire                clk,
    input   wire                resetn,

    input   wire [31:0]         prev_i_data,
    input   wire                prev_i_valid,
    input   wire                next_i_ready,

    //
    output  wire                prev_o_ready,

    output  wire [31:0]         next_o_data,
    output  wire                next_o_valid
);

    reg  [31:0]         prev_i_data_R;
    reg                 prev_i_valid_R;
    reg                 next_i_ready_R;

    always @(posedge clk or negedge clk) begin
        
        if (~resetn) begin

            prev_i_data_R  <= 'b0;
            prev_i_valid_R <= 'b0;

            next_i_ready_R <= 'b0;
        end
        else begin

            prev_i_data_R  <= prev_i_data;
            prev_i_valid_R <= prev_i_valid;

            next_i_ready_R <= next_i_ready;
        end
    end

    common_bypass_buffer #(
        .BUFFER_WIDTH(32)
    ) common_bypass_buffer_INST_SIM (
        .clk(clk),
        .reset(resetn),
        
        .prev_i_data(prev_i_data),
        .prev_i_valid(prev_i_valid),
        .prev_o_ready(prev_o_ready),
        
        .next_o_data(next_o_data),
        .next_o_valid(next_o_valid),
        .next_i_ready(next_i_ready)
    );

endmodule

