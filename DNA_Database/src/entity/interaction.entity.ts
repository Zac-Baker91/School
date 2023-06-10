import { BaseEntity, Column, ColumnTypeUndefinedError, Entity, JoinColumn, JoinTable, ManyToMany, ManyToOne,OneToMany, OneToOne, PrimaryColumn, PrimaryGeneratedColumn } from "typeorm";
import { Cell } from "./cell.entity"
import { Locus } from "./locus.entity";

@Entity()
export class Interaction {
    @PrimaryGeneratedColumn()
    id: number

    @OneToOne(()=>Locus)
    @JoinColumn()
    loc_r: Locus
    
    @OneToOne(()=>Locus)
    @JoinColumn()
    loc_l: Locus

    

}