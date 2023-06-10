import { BaseEntity, Column, ColumnTypeUndefinedError, Entity, JoinColumn, JoinTable, ManyToMany, OneToOne, ManyToOne,OneToMany, PrimaryColumn, PrimaryGeneratedColumn } from "typeorm";
import { Interaction } from "./interaction.entity"
import { Cell } from "./cell.entity";

@Entity()
export class Locus {
    @PrimaryColumn()
    locus_id: number

    @Column()
    start: number

    @Column()
    end: number

    @Column()
    chrnum: number

    @ManyToOne(()=>Cell, (cell)=>cell.type)
    cellType: Cell


}